// #include "Scheduler.h"
#include "src/Server/Server.h"
#include "src/misc/RequestInteger.h"
#include "src/misc/ResponseInteger.h"
#include "src/LoadBalancer/LoadBalancer.h"
#include "src/Scheduler/RoundRobinScheduler.h"

#include <iostream>
#include <memory>


int main() {

    auto load_balancer = LoadBalancer<RequestInteger, ResponseInteger, RoundRobinScheduler<RequestInteger, ResponseInteger>>::create();
    (load_balancer->get_servers()).push_back(Server<RequestInteger, ResponseInteger>::create());

    while (1) {
        RequestInteger request;
        std::cin >> request;

        ResponseInteger response = load_balancer->send_request(request);
        std::cout << response << std::endl;
    }   
    
    return 0;
}
