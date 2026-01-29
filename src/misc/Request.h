#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>

class Request {
private:
    Request(std::string data): data{data} {}
    const std::string data;
public:
    static Request get_request() {     
        std::string data;
        std::cin >> data;
        return Request(data);
    }

    const std::string& get_data() {
        return data;
    }

    Request(Request&) = default;
    Request(Request&&) = default;
};

#endif