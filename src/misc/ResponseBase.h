#ifndef RESPONSEBASE_H
#define RESPONSEBASE_H

#include <iostream>

#include "RequestBase.h"

template<typename ResponseDerived>
class ResponseBase {
public:
    friend std::ostream& operator << (std::ostream& os, const ResponseBase<ResponseDerived>& response) {
        const ResponseDerived& derived = static_cast<const ResponseDerived&>(response);
        os << derived;
        return os;
    }
};

#endif