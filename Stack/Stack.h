//
// Created by Andrey Tabakov on 08/10/2018.
//

#ifndef RCDSTACK_STACK_H
#define RCDSTACK_STACK_H


class Stack {
protected:
    int* stack;

    explicit Stack(int stackSize);

public:
    virtual void push(int value) = 0;
    virtual int pop() = 0;
    virtual int peek() = 0;
    virtual ~Stack();
};


#endif //RCDSTACK_STACK_H
