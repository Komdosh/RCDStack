//
// Created by Andrey Tabakov on 08/10/2018.
//

#include <iostream>
#include "RelaxedStack.h"

RelaxedStack::RelaxedStack(int stackSize, int stacksNum): stacksNum(stacksNum) {
    for(int i = 0; i<stacksNum; ++i){
        stacks[i] = new int[stackSize]{0};
    }

    positions = new int[stacksNum]{0};
}

RelaxedStack::~RelaxedStack() {
    for(int i = 0; i<stacksNum; ++i){
        delete[] stacks[i];
    }
    delete[] stacks;
    delete[] positions;
};

void RelaxedStack::push(int value) {

};

int RelaxedStack::pop() {
    return 0;
};

int RelaxedStack::peek() {
    return 0;
};