#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"

#include <stdexcept>

template<typename ServersRequestType, typename ServersResponseType>
class RoundRobinScheduler: public Scheduler<ServersRequestType, ServersResponseType, RoundRobinScheduler<ServersRequestType, ServersResponseType>> { 
    using ServerType = Server<ServersRequestType, ServersResponseType>;
    using BaseType = Scheduler<ServersRequestType, ServersResponseType, RoundRobinScheduler<ServersRequestType, ServersResponseType>>;
    friend class LoadBalancer<ServersRequestType, ServersResponseType, RoundRobinScheduler<ServersRequestType, ServersResponseType>>;
public:
    std::shared_ptr<ServerType> get_server() {
        if (this->servers.empty()) throw std::runtime_error("no servers are associated with this load balancer");
        if (current_server_idx >= this->servers.size()) current_server_idx = 0;
        auto server = this->servers[current_server_idx];
        if (++current_server_idx >= this->servers.size()) current_server_idx = 0; 
        return server;
    }
private:
    RoundRobinScheduler(const std::vector<std::shared_ptr<ServerType>>& servers): BaseType(servers), current_server_idx(0) {};
    size_t current_server_idx = 0;
};

#endif