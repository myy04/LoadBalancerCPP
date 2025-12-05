// #include "Scheduler.h"
#include "Server/Server.h"
#include "misc/Request.h"
#include "Response.h"
#include "LoadBalancer/LoadBalancer.h"
#include "Scheduler/RoundRobinScheduler.h"

#include <iostream>
#include <vector>
#include <memory>


int main() {
    
    auto load_balancer = LoadBalancer<RoundRobinScheduler>::create();

    for (int i = 1; i <= 5; i++) {
        load_balancer->add_server(std::unique_ptr<Server> (new Server()));
    }

    while (1) {
        Request request = Request::get_request();
        Response response = load_balancer->process_request(request);
        response.print();
    }

    return 0;
}
