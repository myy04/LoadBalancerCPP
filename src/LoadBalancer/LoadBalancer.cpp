#include "LoadBalancer.h"
#include "../Server/Server.h"
#include "../misc/Request.h"
#include "../Scheduler/Scheduler.h"

void LoadBalancerBase::add_server(std::unique_ptr<Server> server) {
    if (!server) throw std::runtime_error("add_server: server is null");
    servers.push_back(std::move(server));  
}

Response LoadBalancerBase::process_request(Request request) {
    auto server = scheduler->next();
    Response response = (*server)->process_request(request);
    return response;
}

LoadBalancerBase::~LoadBalancerBase() = default;