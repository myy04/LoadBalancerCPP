#include "Scheduler.h"
#include "Server.h"
#include "Request.h"
#include "Response.h"
#include "LoadBalancer.h"

#include <iostream>
#include <vector>
#include <memory>


int main() {
    
    auto load_balancer = LoadBalancer<RoundRobinScheduler>::create();

    while (1) {
        Request request = Request::get_request();
        Response response = load_balancer->process_request(request);
        response.print();
    }

    return 0;
}
