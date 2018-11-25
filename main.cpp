#include <iostream>

#include "Stack/Stack.h"

#include "cpu_helper.cpp"
#include "Stack/ThreadSafeStack.h"
#include "Stack/RelaxedStack.h"

#define INSERT_ELEMENT_NUM 2000000
#define ACTION_ELEMENT_NUM 500000
#define EXPERIMENT_NUM 3
#define CORES 6
#define STEP 2
#define CPU_FRQ 2.6E9 //in GHz

using namespace std;

enum ExperimentMode {
    PUSH, POP, PEEK, RANDOM, LAST
};

enum structureType {
    TRADITIONAL, RELAXED
};

struct threadData {
    int threadId;
    ExperimentMode mode;
};


Stack *threadSafeStack;
Stack *relaxedStack;

long *throughputTraditional;
long *throughputRelaxed;

void saveInfo(structureType st, int threadId, uint64_t start) {
    double secs = (__rdtsc() - start) / CPU_FRQ;
    long throughput = static_cast<long>(ACTION_ELEMENT_NUM / secs);
    if (st == TRADITIONAL) {
        throughputTraditional[threadId] = throughput;
    } else if (st == RELAXED) {
        throughputRelaxed[threadId] = throughput;
    }
}

void *RunExperiment(void *threadarg) {
    auto *threadData = (struct threadData *) threadarg;

    if (threadData->mode != PUSH) {
        const int insertNum = threadData->mode != RANDOM ? INSERT_ELEMENT_NUM : ACTION_ELEMENT_NUM;
        for (int i = 0; i < insertNum; ++i) {
            threadSafeStack->push(i);
            relaxedStack->push(i);
        }
    }
    uint64_t start;
    switch (threadData->mode) {
        case PUSH:
            start = __rdtsc();
            for (int i = 0; i < INSERT_ELEMENT_NUM; ++i) {
                threadSafeStack->push(i);
            }
            saveInfo(TRADITIONAL, threadData->threadId, start);
            start = __rdtsc();
            for (int i = 0; i < INSERT_ELEMENT_NUM; ++i) {
                relaxedStack->push(i);
            }
            saveInfo(RELAXED, threadData->threadId, start);
            break;
        case POP:
            start = __rdtsc();
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                relaxedStack->pop();
            }
            saveInfo(TRADITIONAL, threadData->threadId, start);
            start = __rdtsc();
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                relaxedStack->pop();
            }
            saveInfo(RELAXED, threadData->threadId, start);
            break;
        case PEEK:
            start = __rdtsc();
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                threadSafeStack->peek();
            }
            saveInfo(TRADITIONAL, threadData->threadId, start);
            start = __rdtsc();
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                relaxedStack->peek();
            }
            saveInfo(RELAXED, threadData->threadId, start);
            break;
        case RANDOM:
            start = __rdtsc();
            auto seed = new unsigned int[1]{0};
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                switch (rand_r(seed) % 3) {
                    case 0:
                        threadSafeStack->push(i);
                        break;
                    case 1:
                        threadSafeStack->pop();
                        break;
                    case 2:
                        threadSafeStack->peek();
                        break;

                }
            }
            saveInfo(TRADITIONAL, threadData->threadId, start);
            seed = new unsigned int[1]{0};
            start = __rdtsc();
            for (int i = 0; i < ACTION_ELEMENT_NUM; ++i) {
                switch (rand_r(seed) % 3) {
                    case 0:
                        relaxedStack->push(i);
                        break;
                    case 1:
                        relaxedStack->pop();
                        break;
                    case 2:
                        relaxedStack->peek();
                        break;

                }
            }
            saveInfo(RELAXED, threadData->threadId, start);
            break;
    }

    pthread_exit(nullptr);
}

long traditionalTime;
long relaxedTime;

int runForNumOfThreads(ExperimentMode mode, int numOfThreads, int numOfStacksPerThread = 2) {
    cpu_set_t cpuset[CORES];
    for (int i = 0; i < CORES; i++) {
        CPU_ZERO(&cpuset[i]);
        CPU_SET(i, &cpuset[i]);
    }
    pthread_t threads[numOfThreads];
    threadSafeStack = new ThreadSafeStack(INSERT_ELEMENT_NUM * numOfThreads);
    relaxedStack = new RelaxedStack(INSERT_ELEMENT_NUM * numOfThreads, numOfStacksPerThread * numOfThreads);

    throughputTraditional = new long[numOfThreads];
    throughputRelaxed = new long[numOfThreads];

    struct threadData td[numOfThreads];

    for (int i = 0; i < numOfThreads; i++) {
        td[i].threadId = i;
        td[i].mode = mode;
        int rc = pthread_create(&threads[i], nullptr, RunExperiment, (void *) &td[i]);

        int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i % CORES]);
        if (s != 0) {
            printf("Thread %d affinities was not set", i);
            return -1;
        }

        if (rc) {
            cout << "Error: thread wasn't created," << rc << endl;
            return -1;
        }
    }

    for (int i = 0; i < numOfThreads; i++) {
        pthread_join(threads[i], nullptr);
        traditionalTime += throughputTraditional[i];
        relaxedTime += throughputRelaxed[i];
    }

    delete threadSafeStack;
    delete relaxedStack;
    delete[] throughputTraditional;
    delete[] throughputRelaxed;
    return 0;
}


int main(int argc, char *argv[]) {
    int maxThreads = atoi(argv[1]);
    int maxOfStacksPerThread = atoi(argv[2]);

    cout << "[INFO START] Max threads " << maxThreads << endl;
    cout << "[INFO START] Max stacks per thread " << maxOfStacksPerThread << endl;
    cout << "[COMPLETE EXPERIMENT]" << endl;
    cout << "Mode Threads Stacks Traditional Relaxed" << endl << endl;

    for (int mode = PUSH; mode != LAST; ++mode) {
        for (int numOfThreads = 1; numOfThreads < maxThreads; numOfThreads += STEP) {
            for (int numOfStacksPerThread = 1;
                 numOfStacksPerThread < maxOfStacksPerThread; numOfStacksPerThread += STEP) {

                traditionalTime = 0;
                relaxedTime = 0;
                for (int experiment = 0; experiment < EXPERIMENT_NUM; ++experiment) {
                    runForNumOfThreads(static_cast<ExperimentMode>(mode), numOfThreads, numOfStacksPerThread);
                }
                cout << mode << "," << numOfThreads << "," << numOfStacksPerThread << ","
                     << traditionalTime / EXPERIMENT_NUM << "," << relaxedTime / EXPERIMENT_NUM << endl
                     << endl;

                if (numOfStacksPerThread == 1 && STEP > 1) {
                    numOfStacksPerThread = 0;
                }
            }
            if (numOfThreads == 1 && STEP > 1) {
                numOfThreads = 0;
            }
        }
    }


    pthread_exit(nullptr);
}



