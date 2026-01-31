#ifndef REQUESTINTEGER_H
#define REQUESTINTEGER_H

#include "RequestBase.h"

class RequestInteger : public RequestBase<RequestInteger> {
public:
    RequestInteger(): data(0) {};
    RequestInteger(int data): data{data} {};

    friend std::ostream& operator << (std::ostream& os, const RequestInteger& request) {
        os << request.data;
        return os;
    }

    friend std::istream& operator >> (std::istream& is, RequestInteger& request) {
        is >> request.data;
        return is;
    }

    const int get_data() const {
        return data;
    }

protected:
    int data;
};

#endif