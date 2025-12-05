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


#endif
