# C++ TCP Load Balancer

A high-performance Layer 4 (TCP) Load Balancer implemented in C++. This project distributes incoming TCP traffic across multiple backend servers using a Round-Robin scheduling algorithm and includes active health monitoring to ensure reliability.

## 🚀 Features

- **Round-Robin Scheduling**: Evenly distributes requests across all available backend servers.
- **Active Health Checking**: A background thread periodically monitors backend status and automatically removes dead servers from the routing pool.
- **Bi-directional Proxying**: Efficiently pipes data between clients and backends using non-blocking I/O logic (`select`).
- **Real-time CLI Visualization**: Prints a status table every 10 seconds showing backend health and total requests handled.
- **Fault Tolerance**: Automatically redirects traffic when a backend server fails.

## 🛠️ Architecture

- **`LoadBalancer`**: The main entry point that manages the listener socket and coordinates the `BackendManager` and `ProxySession`.
- **`BackendManager`**: Handles the list of backend servers, performs health checks, and implements the Round-Robin logic.
- **`ProxySession`**: Handles the lifecycle of a single client-server connection, forwarding data packets in both directions.
- **`Socket`**: A RAII wrapper around POSIX sockets for safer and cleaner network programming.

## 🏁 Getting Started

### Prerequisites
- CMake 3.10+
- GCC/Clang with C++17 support
- Linux or macOS

### Build and Run
1. **Clone the repository**:
   ```bash
   git clone https://github.com/myy04/LoadBalancerCPP.git
   cd LoadBalancerCPP
   ```

2. **Build the project**:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the Load Balancer**:
   ```bash
   ./load_balancer
   ```
   The Load Balancer will start listening on port `8080`.

## 🧪 Simulation and Testing

To test the load balancer, you can use the provided simulation scripts:

1. **Start Backend Servers**:
   Use the Python script to simulate multiple backend servers on ports 8001, 8002, and 8003.
   ```bash
   python3 simulate_backends.py
   ```

2. **Simulate Client Requests**:
   Use the client script to send a burst of requests to the load balancer.
   ```bash
   python3 simulate_clients.py
   ```

3. **Run Unit Tests**:
   (Unit tests have been removed from the current version of the project)

## 📊 Monitoring
While the load balancer is running, it will output a status table to the console every 10 seconds:
```text
--- Backend Status ---
Backend             Status    Requests
----------------------------------------
127.0.0.1:8001      ALIVE     7
127.0.0.1:8002      ALIVE     7
127.0.0.1:8003      DEAD      0
---------------------
```
