#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Server.h"

#include <memory>
#include <vector>

template<typename RequestType, typename ResponseType>
class LoadBalancerBase {
public:
    virtual ResponseType send_request(RequestType);
protected:
    std::vector<std::shared_ptr<T>> servers;
    LoadBalancerBase() = default;
private:
    using T = LoadBalancerBase<RequestType, ResponseType>;
};

template<typename RequestType, typename ResponseType, typename SchedulingAlgorithm>
class LoadBalancer : public LoadBalancerBase<RequestType, ResponseType> {  
public:
    static std::shared_ptr<T> create();
    virtual ResponseType send_request(RequestType);
protected:
    SchedulingAlgorithm scheduler;
private:
    using T = LoadBalancer<RequestType, ResponseType, SchedulingAlgorithm>;
};

#include "LoadBalancer.ipp"

#endif
