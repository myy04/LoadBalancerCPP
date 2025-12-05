#include "../Server.h"

// class Server {
// public:
//     Server();
//     ~Server();

//     bool has_owner();
//     void set_owner(LoadBalancerBase*);

//     Response process_request(Request);

// private:    
//     friend class LoadBalancerBase;

//     LoadBalancerBase *owner;

//     void set_owner(LoadBalancerBase* new_owner);
// };


Response Server::process_request(Request request) {
    return Response(request.get_data() + (std::string)" processed by a server");
}

void Server::set_owner(LoadBalancerBase* new_owner) {
    if (this->has_owner()) throw std::runtime_error("This server already has an owner");
    this->owner = new_owner;
}

bool Server::has_owner() {
    return this->owner != nullptr;
}

Server::Server(): owner{nullptr}, id{++get_global_counter()} {}