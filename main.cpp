// #include "Scheduler.h"
#include "src/Server/Server.h"
#include "src/misc/Request.h"
#include "src/misc/Response.h"
#include "src/LoadBalancer/LoadBalancer.h"
#include "src/Scheduler/RoundRobinScheduler.h"

#include <iostream>
#include <memory>


int main() {
    
    auto load_balancer = LoadBalancer<RoundRobinScheduler>::create();

    for (int i = 1; i <= 5; i++) {
        load_balancer->add_server(std::unique_ptr<Server> (new Server()));
    }

    while (true) {
        Request request = Request::get_request();
        Response response = load_balancer->process_request(request);
        response.print();
    }

    return 0;
}
