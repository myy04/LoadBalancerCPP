#ifndef LOAD_BALANCER_IPP
#define LOAD_BALANCER_IPP

template<typename RequestType, typename ResponseType, typename SchedulingAlgorithm>
std::shared_ptr<LoadBalancer<RequestType, ResponseType, SchedulingAlgorithm>::create() {
    return std::shared_ptr<LoadBalancer>(load_balancer);
}


#endif
