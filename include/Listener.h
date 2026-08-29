#ifndef LISTENER_H
#define LISTENER_H

#include <string>
#include <sys/socket.h>


class Listener {    
public:
    explicit Listener();
private:
    const int port;
    const std::string ip;

};


#endif