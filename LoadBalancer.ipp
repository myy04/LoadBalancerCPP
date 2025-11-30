#ifndef LOAD_BALANCER_IPP
#define LOAD_BALANCER_IPP

template<typename SchedulingAlgorithm>
std::shared_ptr<LoadBalancer<SchedulingAlgorithm>> LoadBalancer<SchedulingAlgorithm>::create() {
    static_assert(std::is_base_of<Scheduler, SchedulingAlgorithm>::value, "Template parameter SchedulingAlgorithm must be a subclass of Scheduler");

    LoadBalancer* load_balancer = new LoadBalancer();  
    Scheduler* scheduler = new SchedulingAlgorithm(load_balancer);
    load_balancer->scheduler = scheduler;
    
    return std::shared_ptr<LoadBalancer>(load_balancer);
}

void LoadBalancerBase::add_server(std::unique_ptr<Server> server) {
    if (!server) throw std::runtime_error("add_server: server is null");
    if (server->has_owner()) throw std::runtime_error("add_server: server already has an owner");
    
    server->set_owner(this);
    servers.push_back(std::move(server));  
}

Response LoadBalancerBase::process_request(Request request) {
    decltype(scheduler->next()) server = scheduler->next();
    Response response = server->process_request(request);
    return response;
}

#endif
