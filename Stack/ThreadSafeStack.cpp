//
// Created by Andrey Tabakov on 08/10/2018.
//

#include <iostream>
#include "ThreadSafeStack.h"

ThreadSafeStack::ThreadSafeStack(int stackSize) {
    stack = new int[stackSize];
}

ThreadSafeStack::~ThreadSafeStack() {
    delete[] stack;
}

void ThreadSafeStack::push(int value) {
    mutex.lock();

    stack[position] = value;
    position += 1;

    mutex.unlock();
};

int ThreadSafeStack::pop() {
    int value = -1;
    mutex.lock();

    if (position > 0) {
        position -= 1;
        value = stack[position];
    }

    mutex.unlock();
    return value;
};

int ThreadSafeStack::peek() {
    int value = -1;
    mutex.lock();

    if (position > 0) {
        value = stack[position - 1];
    }

    mutex.unlock();
    return value;
}

long ThreadSafeStack::size() {
    return position - 1;
};
