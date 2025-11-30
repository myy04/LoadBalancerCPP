#ifndef SERVER_H
#define SERVER_H

#include "Request.h"
#include "Response.h"
#include <memory>

class LoadBalancerBase;

class Server {
private:    
    void set_owner(LoadBalancerBase* new_owner);
    friend class LoadBalancerBase;
    LoadBalancerBase *owner;
public:
    Server();
    ~Server();

    bool has_owner();
    Response process_request(Request);

};

#endif
