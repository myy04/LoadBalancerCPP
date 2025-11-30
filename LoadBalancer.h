#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "Response.h"
#include "Request.h"
#include <memory>

class Server;

class LoadBalancerBase {
private:
    LoadBalancerBase();
    Scheduler* scheduler;
    std::vector<std::unique_ptr<Server>> servers;
    
public:
    virtual ~LoadBalancerBase() = 0;
    Response process_request(Request);
    void add_server(std::unique_ptr<Server>);

    const std::vector<std::unique_ptr<Server>>& get_servers() {
        return servers;
    }
};

template<typename SchedulingAlgorithm>
class LoadBalancer : public LoadBalancerBase {
public:
    static std::shared_ptr<LoadBalancer<SchedulingAlgorithm>> create();
};


#include "LoadBalancer.ipp"

#endif
