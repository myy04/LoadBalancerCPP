#!/bin/bash

# Build the project
echo "Building project..."
mkdir -p build
cd build
cmake ..
make

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Run the load balancer
echo "Starting Load Balancer..."
./load_balancer
