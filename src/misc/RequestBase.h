#ifndef REQUESTBASE_H
#define REQUESTBASE_H

#include <iostream>

template<typename RequestDerived>
class RequestBase {
public:
    friend std::ostream& operator << (std::ostream& os, const RequestBase<RequestDerived>& request) {
        const RequestDerived& derived = static_cast<const RequestDerived&>(request);
        os << derived;
        return os;
    }  
}; 


#endif