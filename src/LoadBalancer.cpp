#include "LoadBalancer.h"

LoadBalancer::LoadBalancer() : listener_sock_() {
    listener_sock_.set_reuse_addr();
    listener_sock_.bind("0.0.0.0", LB_PORT);
    listener_sock_.listen(10);
    std::cout << "[Load Balancer] Listening on port " << LB_PORT << "...\n";
}

void LoadBalancer::add_backend(const std::string& ip, int port) {
    manager_.add_backend(ip, port);
}

void LoadBalancer::run() {
    std::thread health_thread(&BackendManager::start_health_checker, &manager_);
    health_thread.detach();

    std::thread stats_thread([this]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            this->print_stats();
        }
    });
    stats_thread.detach();

    while (true) {
        try {
            Socket client_sock = listener_sock_.accept();
            
            auto backend_sock_opt = manager_.get_next_backend();
            if (!backend_sock_opt) {
                const char* msg = "HTTP/1.1 503 Service Unavailable\r\n\r\nNo backends available\n";
                client_sock.send(msg, strlen(msg));
                continue;
            }

            ProxySession session(std::move(client_sock), std::move(backend_sock_opt.value()));
            std::thread session_thread(&ProxySession::run, std::move(session));
            session_thread.detach();

        } catch (const std::runtime_error& e) {
            std::cerr << "[Error] " << e.what() << "\n";
        }
    }
}

void LoadBalancer::print_stats() {
    manager_.print_status();
}
