#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Server.h"
#include <vector>
#include "LoadBalancer.h"

class Scheduler {
protected:
    Scheduler() = delete;
    LoadBalancerBase* load_balancer;
    using ContainerType = decltype(load_balancer->get_servers());
    using IteratorType = decltype(load_balancer->get_servers().begin());

public:
    Scheduler(LoadBalancerBase* load_balancer): load_balancer{load_balancer} {}

    virtual IteratorType next() = 0;
    virtual ~Scheduler() = 0;
};

#endif