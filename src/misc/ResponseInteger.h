#ifndef RESPONSEINTEGER_H
#define RESPONSEINTEGER_H

#include "ResponseBase.h"
#include "RequestInteger.h"

class ResponseInteger : ResponseBase<ResponseInteger> {
public:
    ResponseInteger(const RequestInteger& request) {
        data = request.get_data() * 10;
    }

    friend std::ostream& operator << (std::ostream& os, ResponseInteger& response) {
        os << response.data;
        return os;
    }
private:
    int data;
};

#endif