#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <optional>

#define LB_PORT 8080
#define BUFFER_SIZE 4096

// ==========================================
// 1. RAII Socket Wrapper
// ==========================================
class RAIISocket {
private:
    int fd_;

public:
    // Constructor creates the socket
    RAIISocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0) {
        fd_ = socket(domain, type, protocol);
        if (fd_ < 0) throw std::runtime_error("Socket creation failed");
    }

    // Constructor to wrap an existing FD (e.g., from accept())
    explicit RAIISocket(int fd) : fd_(fd) {
        if (fd_ < 0) throw std::runtime_error("Invalid FD provided");
    }

    // Destructor automatically closes the socket (RAII)
    ~RAIISocket() {
        if (fd_ != -1) {
            close(fd_);
        }
    }

    // Disable copy semantics to prevent double-close
    RAIISocket(const RAIISocket&) = delete;
    RAIISocket& operator=(const RAIISocket&) = delete;

    // Implement move semantics
    RAIISocket(RAIISocket&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1; // Prevent the moved-from object from closing it
    }

    RAIISocket& operator=(RAIISocket&& other) noexcept {
        if (this != &other) {
            if (fd_ != -1) close(fd_); // Close our current FD if we are overwriting it
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    // Getters and Socket operations
    int get_fd() const { return fd_; }

    void set_reuse_addr() {
        int opt = 1;
        setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    void bind(const std::string& ip, int port) {
        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

        if (::bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
            throw std::runtime_error("Bind failed");
        }
    }

    void listen(int backlog = 10) {
        if (::listen(fd_, backlog) < 0) {
            throw std::runtime_error("Listen failed");
        }
    }

    RAIISocket accept() {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = ::accept(fd_, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            throw std::runtime_error("Accept failed");
        }
        return RAIISocket(client_fd); // Return wrapped socket
    }

    void connect(const std::string& ip, int port) {
        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

        if (::connect(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
            throw std::runtime_error("Connect failed");
        }
    }

    ssize_t send(const char* buf, size_t len) {
        return ::send(fd_, buf, len, 0);
    }

    ssize_t recv(char* buf, size_t len) {
        return ::recv(fd_, buf, len, 0);
    }
};

// ==========================================
// 2. Backend Manager
// ==========================================
struct Backend {
    std::string ip;
    int port;
    bool is_alive;
};

class BackendManager {
private:
    std::vector<Backend> backends_;
    std::mutex mtx_;
    int current_idx_ = 0;

public:
    void add_backend(const std::string& ip, int port) {
        backends_.push_back({ip, port, true});
    }

    // Returns a connected socket to an alive backend, or std::nullopt if all are dead
    std::optional<RAIISocket> get_next_backend() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (backends_.empty()) return std::nullopt;

        for (int i = 0; i < backends_.size(); ++i) {
            int idx = current_idx_ % backends_.size();
            current_idx_++;

            if (backends_[idx].is_alive) {
                try {
                    RAIISocket sock;
                    sock.connect(backends_[idx].ip, backends_[idx].port);
                    std::cout << "[Routing] Forwarding to " << backends_[idx].ip << ":" << backends_[idx].port << "\n";
                    return sock; // Move semantics handle transfer of ownership
                } catch (const std::runtime_error& e) {
                    // Connection failed, try next backend
                    std::cerr << "[Warning] Failed to connect to " << backends_[idx].ip << ":" << backends_[idx].port << "\n";
                }
            }
        }
        return std::nullopt; // All backends failed
    }

    void start_health_checker() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            for (auto& b : backends_) {
                bool alive = false;
                try {
                    RAIISocket sock;
                    sock.connect(b.ip, b.port);
                    alive = true; // If connect succeeds, it's alive. RAII will close it here.
                } catch (...) {
                    alive = false;
                }

                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    if (b.is_alive != alive) {
                        std::cout << "[Health] " << b.ip << ":" << b.port << " is now " 
                                  << (alive ? "ALIVE" : "DEAD") << "\n";
                        b.is_alive = alive;
                    }
                }
            }
        }
    }
};

// ==========================================
// 3. Proxy Session (Handles the Data Bridge)
// ==========================================
class ProxySession {
private:
    RAIISocket client_sock_;
    RAIISocket backend_sock_;

public:
    // Takes ownership of both sockets via move semantics
    ProxySession(RAIISocket client, RAIISocket backend) 
        : client_sock_(std::move(client)), backend_sock_(std::move(backend)) {}

    void run() {
        char buffer[BUFFER_SIZE];
        
        while (true) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(client_sock_.get_fd(), &read_fds);
            FD_SET(backend_sock_.get_fd(), &read_fds);

            int max_fd = std::max(client_sock_.get_fd(), backend_sock_.get_fd());
            int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
            if (activity < 0) break;

            // Client -> Backend
            if (FD_ISSET(client_sock_.get_fd(), &read_fds)) {
                ssize_t bytes = client_sock_.recv(buffer, BUFFER_SIZE);
                if (bytes <= 0) break;
                backend_sock_.send(buffer, bytes);
            }

            // Backend -> Client
            if (FD_ISSET(backend_sock_.get_fd(), &read_fds)) {
                ssize_t bytes = backend_sock_.recv(buffer, BUFFER_SIZE);
                if (bytes <= 0) break;
                client_sock_.send(buffer, bytes);
            }
        }
        // Destructors of client_sock_ and backend_sock_ will close the connections automatically here
    }
};



// ==========================================
// Main Entry Point
// ==========================================
int main() {
    try {
        LoadBalancer lb;
        lb.add_backend("127.0.0.1", 8001);
        lb.add_backend("127.0.0.1", 8002);
        lb.add_backend("127.0.0.1", 8003);
        
        lb.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}