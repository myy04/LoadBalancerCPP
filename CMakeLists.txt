cmake_minimum_required(VERSION 3.31)

project(LoadBalancerCPP)

set(CMAKE_CXX_STANDARD 11)

add_executable(LoadBalancerCPP
    main.cpp
)

target_include_directories(LoadBalancerCPP PRIVATE ${CMAKE_SOURCE_DIR}/src)