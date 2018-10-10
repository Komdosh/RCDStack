//
// Created by Andrey Tabakov on 08/10/2018.
//

#include <iostream>
#include "ThreadSafeStack.h"

ThreadSafeStack::ThreadSafeStack(int stackSize):Stack(stackSize) {
}

void ThreadSafeStack::push(int value) {
    mutex.lock();

    stack[position] = value;
    position += 1;

    mutex.unlock();
};

int ThreadSafeStack::pop() {
    int value;
    mutex.lock();

    value = stack[position];
    position -= 1;

    mutex.unlock();
    return value;
};

int ThreadSafeStack::peek() {
    int value;
    mutex.lock();

    value = stack[position-1];

    mutex.unlock();
    return value;
};