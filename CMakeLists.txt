cmake_minimum_required(VERSION 3.12)
project(RCDStack)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS -pthread)

set(SOURCE ${SOURCE} Stack/Stack.cpp)
set(SOURCE ${SOURCE} Stack/ThreadSafeStack.cpp)
set(SOURCE ${SOURCE} Stack/RelaxedStack.cpp)

find_package(Boost 1.58.0 COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(RCDStack ${HEADERS} ${SOURCE} main.cpp)
    target_link_libraries(RCDStack ${Boost_LIBRARIES})
endif()