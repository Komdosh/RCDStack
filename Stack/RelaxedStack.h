//
// Created by Andrey Tabakov on 08/10/2018.
//

#ifndef RCDSTACK_RELAXEDSTACK_H
#define RCDSTACK_RELAXEDSTACK_H

#include "Stack.h"

class RelaxedStack: public Stack {
public:
    explicit RelaxedStack(int stackSize);
    void push(int num) override;
    int pop() override;
    int peek() override;
};


#endif //RCDSTACK_RELAXEDSTACK_H
