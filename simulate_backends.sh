#!/bin/bash

# Ports defined in main.cpp
PORTS=(8001 8002 8003)
PIDS=()

echo "Starting simulation of backend servers..."

for PORT in "${PORTS[@]}"; do
    # Use nc (netcat) to create a simple listener that responds with a message
    # -l: listen, -p: port, -k: keep open
    # We use a loop with nc to simulate a server that stays alive
    (while true; do echo -e "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello from $PORT" | nc -l $PORT; done) &
    PID=$!
    PIDS+=($PID)
    echo "Backend server started on port $PORT (PID: $PID)"
done

echo "Backend servers are running. Press Ctrl+C to stop all of them."

# Handle shutdown
trap "echo 'Stopping backend servers...'; kill ${PIDS[*]}; exit" SIGINT SIGTERM

# Keep script running
wait
