#include "Socket.h"

Socket::Socket(int domain, int type, int protocol) {
    fd_ = socket(domain, type, protocol);
    if (fd_ < 0) throw std::runtime_error("Socket creation failed");
}

Socket::Socket(int fd, bool is_fd) : fd_(fd) {
    if (fd_ < 0) throw std::runtime_error("Invalid FD provided");
}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
    }
}

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (fd_ != -1) close(fd_);
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

int Socket::get_fd() const { return fd_; }

void Socket::set_reuse_addr() {
    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::bind(const std::string& ip, int port) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (::bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("Bind failed");
    }
}

void Socket::listen(int backlog) {
    if (::listen(fd_, backlog) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

Socket Socket::accept() {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = ::accept(fd_, (sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        throw std::runtime_error("Accept failed");
    }
    return Socket(client_fd, true);
}

void Socket::connect(const std::string& ip, int port) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (::connect(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("Connect failed");
    }
}

ssize_t Socket::send(const char* buf, size_t len) {
    return ::send(fd_, buf, len, 0);
}

ssize_t Socket::recv(char* buf, size_t len) {
    return ::recv(fd_, buf, len, 0);
}
