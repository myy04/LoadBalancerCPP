#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <iostream>

class Response {
public:
    Response(Response&&);
    Response(Response&);
    
    Response(std::string data): data{data} {}

    void print() {
        std::cout << this->data << std::endl;
    }

private:
    const std::string data;
};

#endif