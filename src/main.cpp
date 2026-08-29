#include "LoadBalancer.h"
#include <iostream>

int main() {
    try {
        LoadBalancer lb;
        lb.add_backend("127.0.0.1", 8001);
        lb.add_backend("127.0.0.1", 8002);
        lb.add_backend("127.0.0.1", 8003);
        
        lb.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
