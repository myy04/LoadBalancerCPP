#ifndef PROXY_SESSION_H
#define PROXY_SESSION_H

#include "Socket.h"
#include <algorithm>
#include <sys/select.h>

#define BUFFER_SIZE 4096

class ProxySession {
private:
    Socket client_sock_;
    Socket backend_sock_;

public:
    ProxySession(Socket client, Socket backend) 
        : client_sock_(std::move(client)), backend_sock_(std::move(backend)) {}

    void run();
};

#endif
