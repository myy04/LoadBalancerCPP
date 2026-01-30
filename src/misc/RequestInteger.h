#ifndef REQUESTINTEGER_H
#define REQUESTINTEGER_H

#include "RequestBase.h"

class RequestInteger : public RequestBase<RequestInteger> {
public:
    RequestInteger() = delete;
    RequestInteger(int data): data{data} {};

    friend std::ostream& operator << (std::ostream& os, const RequestInteger& request) {
        os << request.data;
        return os;
    }

    const int get_data() const {
        return data;
    }

private:
    int data;
};

#endif