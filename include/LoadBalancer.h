#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "BackendManager.h"
#include "Socket.h"
#include "ProxySession.h"
#include <string>
#include <iostream>
#include <thread>
#include <cstring>

#define LB_PORT 8080

class LoadBalancer {
private:
    BackendManager manager_;
    Socket listener_sock_;

public:
    LoadBalancer();
    void add_backend(const std::string& ip, int port);
    void run();
    void print_stats();
};

#endif
