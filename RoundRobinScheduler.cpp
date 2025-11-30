#include "Scheduler.h"

class RoundRobinScheduler: public Scheduler { 
private:
    const ContainerType& servers;
    IteratorType next_server;
public:
    virtual IteratorType next() override {
        auto ret = next_server++;
        if (next_server == servers.end()) next_server = servers.begin();
        return ret;
    }

    RoundRobinScheduler(LoadBalancerBase* lb):
        Scheduler(lb),
        servers{load_balancer->get_servers()}, 
        next_server{load_balancer->get_servers().begin()} {}
};