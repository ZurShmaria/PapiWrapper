#include <iostream>
#include "../include/papiwrapper.h"
#include <omp.h>
#include <pthread.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>

#define TEST_SIZE 128000

void doFlops();
void doReads();
void doMisses();

void testSingle();
void testParallel();

int main()
{
    std::cout << "==========================> Test Single <===========================" << std::endl;
    testSingle();
    std::cout << "==========================> Test Parallel <===========================" << std::endl;
    testParallel();
}

void testSingle()
{
    PAPIW::INIT(PAPI_L2_TCA, PAPI_L2_TCM, PAPI_L3_TCA, PAPI_L3_TCM);

    std::cout << "==========================> Do Flops <===========================" << std::endl;
    PAPIW::START();
    doFlops();
    PAPIW::STOP();
    PAPIW::PRINT();

    PAPIW::RESET();

    std::cout << "==========================> Do Reads <===========================" << std::endl;
    PAPIW::START();
    doReads();
    PAPIW::STOP();
    PAPIW::PRINT();

    PAPIW::RESET();

    std::cout << "==========================> Do Misses <===========================" << std::endl;
    PAPIW::START();
    doMisses();
    PAPIW::STOP();
    PAPIW::PRINT();
}

void testParallel()
{
    PAPIW::INIT<32>(PAPI_L2_TCA, PAPI_L2_TCM, PAPI_L3_TCA, PAPI_L3_TCM);

    PAPIW::START();
#pragma omp parallel
    {
        doReads();
    }
    PAPIW::STOP();
    std::cout << "==========================> Do Reads <===========================" << std::endl;
    PAPIW::PRINT();

    PAPIW::RESET();

    PAPIW::START();
#pragma omp parallel
    {
        doMisses();
    }
    PAPIW::STOP();
    std::cout << "==========================> Do Misses <===========================" << std::endl;
    PAPIW::PRINT();
}

void dummy(void *array)
{
    (void)array;
}

void doFlops()
{
    double acc = 0.11;
    for (int i = 0; i < TEST_SIZE; i++)
    {
        acc += 0.3 * 1.4;
    }
    dummy((void *)&acc);
}

void doReads()
{
    long long *a = new long long[TEST_SIZE];
    for (int i = 0; i < TEST_SIZE; i++)
        a[i] = i;

    long long *acc = new long long[TEST_SIZE];
    for (int i = 0; i < TEST_SIZE; i++)
        acc[i] = a[i] + i;

    dummy((void *)acc);
    delete a;
    delete acc;
}

void doMisses()
{
    long long *a = new long long[TEST_SIZE];
    for (int i = 0; i < TEST_SIZE; i++)
        a[i] = i;

    long long *acc = new long long[TEST_SIZE];
    for (int i = 0; i < TEST_SIZE; i++)
        acc[i] = a[rand() % TEST_SIZE] + i;

    dummy((void *)acc);
    delete a;
    delete acc;
}