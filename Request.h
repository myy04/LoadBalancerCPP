#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>

class Request {
private:
    Request(std::string data): data{data} {}
    std::string data;
public:
    static Request get_request() {     
        std::string data;
        std::cin >> data;
        return Request(data);
    }

    Request(Request&);
    Request(Request&&);
};

#endif