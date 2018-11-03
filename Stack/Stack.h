//
// Created by Andrey Tabakov on 08/10/2018.
//

#ifndef RCDSTACK_STACK_H
#define RCDSTACK_STACK_H


class Stack {
public:
    virtual void push(int value) = 0;
    virtual int pop() = 0;
    virtual int peek() = 0;

    virtual long size() = 0;
    virtual ~Stack() = 0;
};


#endif //RCDSTACK_STACK_H
