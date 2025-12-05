#ifndef SERVER_H
#define SERVER_H

#include "../misc/Request.h"
#include "../Response.h"
#include <memory>

class LoadBalancerBase;

class Server {
private:    
    void set_owner(LoadBalancerBase* new_owner);
    friend class LoadBalancerBase;
    LoadBalancerBase *owner;
    
    static size_t& get_global_counter() {
        static size_t global_counter;
        return global_counter;
    }

public:
    Server();
    ~Server();

    bool has_owner();
    Response process_request(Request);
    const size_t id;
};

#endif
