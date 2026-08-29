#include "ProxySession.h"
#include <iostream>

void ProxySession::run() {
    char buffer[BUFFER_SIZE];
    
    while (true) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_sock_.get_fd(), &read_fds);
        FD_SET(backend_sock_.get_fd(), &read_fds);

        int max_fd = std::max(client_sock_.get_fd(), backend_sock_.get_fd());
        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0) break;

        if (FD_ISSET(client_sock_.get_fd(), &read_fds)) {
            ssize_t bytes = client_sock_.recv(buffer, BUFFER_SIZE);
            if (bytes <= 0) break;
            backend_sock_.send(buffer, bytes);
        }

        if (FD_ISSET(backend_sock_.get_fd(), &read_fds)) {
            ssize_t bytes = backend_sock_.recv(buffer, BUFFER_SIZE);
            if (bytes <= 0) break;
            client_sock_.send(buffer, bytes);
        }
    }
}
