#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"

class RoundRobinScheduler: public Scheduler { 
private:
    ContainerType& const servers;
    IteratorType next_server;
public:
    virtual IteratorType next() override;
    RoundRobinScheduler(LoadBalancerBase* lb);
};

#endif