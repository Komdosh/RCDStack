#include <iostream>

#include "Stack/Stack.h"
#include "Stack/ThreadSafeStack.h"
#include "Stack/RelaxedStack.h"

#include <x86intrin.h>
#include <zconf.h>
#include "mpi.h"
#include "cpu_helper.cpp"

#define NUM_ELEMENT 500000
#define CORES 6
#define NUM_OF_THREADS 2

using namespace std;

struct threadData {
    int threadId;
};

Stack* threadSafeStack;
Stack* relaxedStack;

void *RunExperiment(void *threadarg) {
    struct threadData *threadData = (struct threadData *) threadarg;

    for(int i = 0; i<NUM_ELEMENT; ++i){
        threadSafeStack->push(i);
        relaxedStack->push(i);
    }

    cout<<"ThreadSafeStack Peek: "<<threadSafeStack->peek()<<endl;
    cout<<"ThreadSafeStack Pop: "<<threadSafeStack->pop()<<endl;

//    cout<<"RelaxedStack Peek: "<<relaxedStack->peek()<<endl;
//    cout<<"RelaxedStack Pop: "<<relaxedStack->pop()<<endl;

    pthread_exit(nullptr);
}


int main() {


    cpu_set_t cpuset[CORES];
    pthread_t threads[NUM_OF_THREADS];
    for (int i = 0; i < CORES; i++) {
        CPU_ZERO(&cpuset[i]);
        CPU_SET(i, &cpuset[i]);
    }

    threadSafeStack = new ThreadSafeStack(NUM_ELEMENT*NUM_OF_THREADS);
    relaxedStack = new RelaxedStack(NUM_ELEMENT*NUM_OF_THREADS);

    struct threadData td[NUM_OF_THREADS];

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        td[i].threadId = i;

        int rc = pthread_create(&threads[i], nullptr, RunExperiment, (void *) &td[i]);

        int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i % CORES]);
        if (s != 0) {
            printf("Thread %d affinities was not set", i);
            pthread_exit(nullptr);
        }

        if (rc) {
            cout << "Error: thread wasn't created," << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    delete threadSafeStack;
    delete relaxedStack;

    pthread_exit(nullptr);
}

