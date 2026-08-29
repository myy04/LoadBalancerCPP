#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

class Socket {
private:
    int fd_;

public:
    Socket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    explicit Socket(int fd, bool is_fd = true);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    int get_fd() const;
    void set_reuse_addr();
    void bind(const std::string& ip, int port);
    void listen(int backlog = 10);
    Socket accept();
    void connect(const std::string& ip, int port);
    ssize_t send(const char* buf, size_t len);
    ssize_t recv(char* buf, size_t len);
};

#endif
