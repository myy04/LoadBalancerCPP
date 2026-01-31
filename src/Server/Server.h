#ifndef SERVER_H
#define SERVER_H

#include "misc/RequestBase.h"
#include "misc/ResponseBase.h"

#include <memory>
#include <random>
#include <chrono>
#include <thread>

class ServerBase {
protected:
    ServerBase() {
        get_counter()++;
    }

    static int& get_counter() {
        static int counter = 0;
        return counter;
    }
};

template<typename RequestType, typename ResponseType>
class Server : ServerBase {
public:
    static std::shared_ptr<Server> create();
    ResponseType send_request(RequestType);
private:
    Server();
    const int MAX_DELAY = 7;
    const int MIN_DELAY = 1;
    const int server_id;
    const int delay;
};


#include "Server.ipp"

#endif
