#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Server/Server.h"
#include "LoadBalancer/LoadBalancer.h"
#include <vector>
 
template<typename ServersRequestType, typename ServersResponseType, typename SchedulingAlgorithm>
class Scheduler {    
    using ServerType = Server<ServersRequestType, ServersResponseType>;
public:
    std::shared_ptr<ServerType> get_server() {
        return (static_cast<SchedulingAlgorithm*>(this))->get_server();
    }
protected: 
    Scheduler() = default;
    Scheduler(const std::vector<std::shared_ptr<ServerType>>& servers): servers{servers} {}
    const std::vector<std::shared_ptr<ServerType>>& servers;
};

#endif