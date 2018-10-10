//
// Created by Andrey Tabakov on 08/10/2018.
//

#ifndef RCDSTACK_THREADSAFESTACK_H
#define RCDSTACK_THREADSAFESTACK_H

#include "Stack.h"
#include <mutex>

class ThreadSafeStack: public Stack {
    std::mutex mutex;
    volatile int position;
public:
    explicit ThreadSafeStack(int stackSize);
    void push(int value) override;
    int pop() override;
    int peek() override;
};


#endif //RCDSTACK_THREADSAFESTACK_H
