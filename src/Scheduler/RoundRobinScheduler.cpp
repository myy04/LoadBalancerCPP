#include "RoundRobinScheduler.h"

const Server* RoundRobinScheduler::next() {
    auto ret = next_server++;
    if (next_server == servers.end()) next_server = servers.begin();
    return ret;
}

RoundRobinScheduler::RoundRobinScheduler(LoadBalancerBase* lb):
    Scheduler(lb),
    servers{load_balancer->get_servers()}, 
    next_server{load_balancer->get_servers().begin()} {}