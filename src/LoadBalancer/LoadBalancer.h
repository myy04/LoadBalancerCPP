#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Server/Server.h"

#include <memory>
#include <vector>

template<typename RequestType, typename ResponseType>
class LoadBalancerBase {
    using ServerType = Server<RequestType, ResponseType>;
public:
    virtual ResponseType send_request(RequestType) = 0;
    std::vector<std::shared_ptr<ServerType>>& get_servers() {return servers;}
    virtual ~LoadBalancerBase() = default;    
protected:
    std::vector<std::shared_ptr<ServerType>> servers;
    LoadBalancerBase() = default;
private:
};

template<typename RequestType, typename ResponseType, typename SchedulingAlgorithm>
class LoadBalancer : public LoadBalancerBase<RequestType, ResponseType> {  
    using T = LoadBalancer<RequestType, ResponseType, SchedulingAlgorithm>;
    using ServerType = Server<RequestType, ResponseType>;
public:
    static std::shared_ptr<T> create() {
        return std::shared_ptr<T>(new T());
    }
    virtual ResponseType send_request(RequestType request) {
        std::shared_ptr<ServerType> server = scheduler.get_server();
        ResponseType response = server->send_request(request);
        return response;
    }
protected:
    SchedulingAlgorithm scheduler;
    LoadBalancer(): scheduler(this->servers) {};
};

#endif
