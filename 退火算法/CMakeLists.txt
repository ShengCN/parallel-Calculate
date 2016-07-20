cmake_minimum_required(VERSION 3.5)
project(TSP_Problem_SimulatedAnnealing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Population.cpp Population.h Tour.cpp Tour.h SimulatedAnnealing.cpp SimulatedAnnealing.h)
add_executable(TSP_Problem_SimulatedAnnealing ${SOURCE_FILES})