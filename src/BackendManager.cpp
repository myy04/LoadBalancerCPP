#include "BackendManager.h"
#include <iomanip>
#include <thread>
#include <chrono>

BackendManager::~BackendManager() {
    for (auto b : backends_) delete b;
}

void BackendManager::add_backend(const std::string& ip, int port) {
    backends_.push_back(new Backend{ip, port, true});
}

std::optional<Socket> BackendManager::get_next_backend() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (backends_.empty()) return std::nullopt;

    for (size_t i = 0; i < backends_.size(); ++i) {
        int idx = current_idx_ % backends_.size();
        current_idx_++;

        if (backends_[idx]->is_alive) {
            try {
                Socket sock;
                sock.connect(backends_[idx]->ip, backends_[idx]->port);
                backends_[idx]->requests_handled++;
                return sock;
            } catch (const std::runtime_error& e) {
                // Connection failed, mark as dead and try next
                backends_[idx]->is_alive = false;
            }
        }
    }
    return std::nullopt;
}

void BackendManager::start_health_checker() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        for (auto b : backends_) {
            bool alive = false;
            try {
                Socket sock;
                sock.connect(b->ip, b->port);
                alive = true;
            } catch (...) {
                alive = false;
            }

            {
                std::lock_guard<std::mutex> lock(mtx_);
                if (b->is_alive != alive) {
                    b->is_alive = alive;
                }
            }
        }
    }
}

void BackendManager::print_status() {
    std::lock_guard<std::mutex> lock(mtx_);
    std::cout << "\n--- Backend Status ---\n";
    std::cout << std::left << std::setw(20) << "Backend" << std::setw(10) << "Status" << "Requests\n";
    std::cout << std::string(40, '-') << "\n";
    for (auto b : backends_) {
        std::cout << std::left << std::setw(20) << (b->ip + ":" + std::to_string(b->port))
                  << std::setw(10) << (b->is_alive ? "ALIVE" : "DEAD")
                  << b->requests_handled << "\n";
    }
    std::cout << "---------------------\n";
}

