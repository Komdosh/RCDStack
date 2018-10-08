#include <iostream>

#include "Stack/Stack.h"
#include "Stack/ThreadSafeStack.h"
#include "Stack/RelaxedStack.h"

#define STACK_SIZE 16

int main() {
    Stack* threadSafeStack = new ThreadSafeStack(STACK_SIZE);
    Stack* relaxedStack = new RelaxedStack(STACK_SIZE);

    delete threadSafeStack;
    delete relaxedStack;

    return 0;
}