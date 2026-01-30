#ifndef SERVER_IPP
#define SERVER_IPP

#include "Server.h"

template<typename RequestType, typename ResponseType>
std::shared_ptr<Server<RequestType, ResponseType>> Server<RequestType, ResponseType>::create() {
    return std::make_shared<Server<RequestType, ResponseType>>();
}

template<typename RequestType, typename ResponseType>
ResponseType Server<RequestType, ResponseType>::send_request(RequestType request) {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    return ResponseType(request);
}

template<typename RequestType, typename ResponseType>
Server<RequestType, ResponseType>::Server() : delay(std::max(MIN_DELAY, rand() % MAX_DELAY)) {
    static int servers_counter = 0;
    server_id = ++servers_counter;
    std::cout << "Server (" << server_id << ") " << "has a delay: " << delay << " seconds" << std::endl;
}

#endif