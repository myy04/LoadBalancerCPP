#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"

#include <stdexcept>

template<typename ServersRequestType, typename ServersResponseType>
class RoundRobinScheduler: public Scheduler<ServersRequestType, ServersResponseType, RoundRobinScheduler<ServersRequestType, ServersResponseType>> { 
    using ServerType = Server<ServersRequestType, ServersResponseType>;
    using BaseType = Scheduler<ServersRequestType, ServersResponseType, RoundRobinScheduler<ServersRequestType, ServersResponseType>>;
public:
    std::shared_ptr<ServerType> get_server() {
        if (this->servers.empty()) throw std::runtime_error("no servers are associated with this load balancer");
        auto ret = *current_server_iterator;
        current_server_iterator++;
        if (current_server_iterator == this->servers.end()) current_server_iterator = this->servers.begin();
        return ret;
    }
    RoundRobinScheduler(const std::vector<std::shared_ptr<ServerType>>& servers): BaseType(servers), current_server_iterator(servers.end()) {};
private:
    typename std::vector<std::shared_ptr<ServerType>>::const_iterator current_server_iterator;
};

#endif