cmake_minimum_required(VERSION 3.12)
project(RCDStack)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS -pthread)

set(SOURCE ${SOURCE} Stack/Stack.cpp)
set(SOURCE ${SOURCE} Stack/ThreadSafeStack.cpp)
set(SOURCE ${SOURCE} Stack/RelaxedStack.cpp)

add_executable(RCDStack ${HEADERS} ${SOURCE} main.cpp)