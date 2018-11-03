//
// Created by Andrey Tabakov on 08/10/2018.
//

#include <iostream>
#include "RelaxedStack.h"

RelaxedStack::RelaxedStack(int stackSize, int stacksNum) : stacksNum(stacksNum) {
    stacks = new int *[stacksNum];
    for (int i = 0; i < stacksNum; ++i) {
        stacks[i] = new int[stackSize]{0};
    }
    positions = new int[stacksNum]{0};
    locks = new std::mutex[stacksNum];
}

RelaxedStack::~RelaxedStack() {
    for (int i = 0; i < stacksNum; ++i) {
        delete[] stacks[i];
    }
    delete[] stacks;
    delete[] positions;
    delete[] locks;
};

void RelaxedStack::push(int value) {
    int id = getStackId();
    int position = positions[id];

    stacks[id][position] = value;
    positions[id] = position + 1;

    locks[id].unlock();
};

int RelaxedStack::pop() {
    int value = -1;
    int id = getStackId();
    int position = positions[id] - 1;

    if (position >= 0) {
        value = stacks[id][position];
        positions[id] = position;
    }

    locks[id].unlock();
    return value;
};

int RelaxedStack::peek() {
    int value = -1;
    int id = getStackId();
    int position = positions[id] - 1;

    if (position >= 0) {
        value = stacks[id][position];
    }

    locks[id].unlock();
    return value;
}

int RelaxedStack::getStackId() {
    int id;
    do {
        id = getRandomStackNum();
    } while (!locks[id].try_lock());
    return id;
}

long RelaxedStack::size() {
    long numOfElements = 0;
    for (int i = 0; i < this->stacksNum; ++i) {
        numOfElements += positions[i] - 1;
    }
    return numOfElements;
};


int RelaxedStack::getRandomStackNum() {
    return rand_r(this->seed) % stacksNum;
};

