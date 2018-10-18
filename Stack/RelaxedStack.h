//
// Created by Andrey Tabakov on 08/10/2018.
//

#ifndef RCDSTACK_RELAXEDSTACK_H
#define RCDSTACK_RELAXEDSTACK_H

#include "Stack.h"
#include <mutex>

class RelaxedStack: public Stack {
    int* positions;
    int** stacks;
    int stacksNum;
    unsigned int *seed = new unsigned int[1]{0};
    std::mutex *locks;

    int getRandomStackNum();

    int getStackId();
public:
    explicit RelaxedStack(int stackSize, int stacksNum = 8);
    void push(int value) override;
    int pop() override;
    int peek() override;
    ~RelaxedStack() override;
};


#endif //RCDSTACK_RELAXEDSTACK_H
