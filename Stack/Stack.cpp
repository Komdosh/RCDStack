//
// Created by Andrey Tabakov on 08/10/2018.
//

#include "Stack.h"

#include <iostream>

Stack::Stack(int stackSize) {
    stack = new int[stackSize];
}

Stack::~Stack() {
    delete[] stack;
}
