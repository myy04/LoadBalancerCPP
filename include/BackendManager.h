#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

#include "Socket.h"
#include <string>
#include <vector>
#include <mutex>
#include <optional>
#include <iostream>
#include <atomic>
#include <map>

struct Backend {
    std::string ip;
    int port;
    bool is_alive;
    std::atomic<uint64_t> requests_handled{0};
};

class BackendManager {
private:
    std::vector<Backend*> backends_;
    std::mutex mtx_;
    int current_idx_ = 0;

public:
    ~BackendManager();
    void add_backend(const std::string& ip, int port);
    std::optional<Socket> get_next_backend();
    void start_health_checker();
    void print_status();
};

#endif
