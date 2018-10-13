//
// Created by Andrey Tabakov on 08/10/2018.
//

#include <iostream>
#include "ThreadSafeStack.h"

using namespace std;

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