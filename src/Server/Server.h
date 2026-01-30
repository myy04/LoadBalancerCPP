#ifndef SERVER_H
#define SERVER_H

#include "../misc/RequestBase.h"
#include "../misc/ResponseBase.h"

#include <memory>
#include <random>
#include <chrono>
#include <thread>

template<typename RequestType, typename ResponseType>
class Server {
public:
    static std::shared_ptr<Server> create() {
        return std::shared_ptr<Server<RequestType, ResponseType>> Server<RequestType, ResponseType>();
    }

    ResponseType send_request(RequestType request) {
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        return ResponseType(request);
    }

private:
    Server(): delay(max(MIN_DELAY, rand() % MAX_DELAY)) {
        static int servers_counter = 0;
        server_id = ++servers_counter;
        std::cout << "Server (" << server_id << ") " << "has a delay: " << delay << " seconds" << std::endl;
    };

    const int server_id;
    const int delay;
    const static int MAX_DELAY = 7;
    const static int MIN_DELAY = 1;
};


#include "Server.ipp"

#endif
