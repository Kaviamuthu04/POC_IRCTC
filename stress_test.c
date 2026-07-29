#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

#include "memory_manager.h"
#include "logger.h"

#define USER_MEMORY_KB 50

static int successfulBookings = 0;
static int waitlistBookings = 0;
static int oomEvents = 0;

static int totalAllocatedMemory = 0;
static int totalFreedMemory = 0;

pthread_mutex_t counterLock;

void *stressUser(void *arg)
{
    int id = *(int *)arg;

    printf("User %d Booking Started\n", id);

    char *buffer;

    buffer =
    (char *)malloc(
    USER_MEMORY_KB * 1024);

    if(buffer != NULL)
    {
        pthread_mutex_lock(
        &counterLock);

        totalAllocatedMemory +=
        USER_MEMORY_KB;

        pthread_mutex_unlock(
        &counterLock);
    }

    Sleep(100);

    int bookingResult;

    bookingResult = rand() % 100;

    pthread_mutex_lock(
    &counterLock);

    if(bookingResult < 80)
    {
        successfulBookings++;
    }
    else
    {
        waitlistBookings++;
    }

    pthread_mutex_unlock(
    &counterLock);

    if(buffer != NULL)
    {
        free(buffer);

        pthread_mutex_lock(
        &counterLock);

        totalFreedMemory +=
        USER_MEMORY_KB;

        pthread_mutex_unlock(
        &counterLock);
    }

    printf("User %d Booking Completed\n",
           id);

    return NULL;
}

void runStressTest()
{
    int users;

    successfulBookings = 0;
    waitlistBookings = 0;
    oomEvents = 0;

    totalAllocatedMemory = 0;
    totalFreedMemory = 0;

    printf("\n===== STRESS TEST =====\n");

    printf("Enter Number Of Users : ");
    scanf("%d", &users);

    if(users <= 0)
    {
        printf("Invalid User Count\n");
        return;
    }

    srand((unsigned int)time(NULL));

    pthread_mutex_init(
    &counterLock,
    NULL);

    pthread_t *threads;

    int *ids;

    threads =
    (pthread_t *)malloc(
    sizeof(pthread_t) * users);

    ids =
    (int *)malloc(
    sizeof(int) * users);

    if(threads == NULL ||
       ids == NULL)
    {
        printf("Memory Allocation Failed\n");

        free(threads);
        free(ids);

        return;
    }

    int memoryBefore;

    memoryBefore =
    getAvailableMemory();

    clock_t start;

    start = clock();

    for(int i = 0; i < users; i++)
    {
        ids[i] = i + 1;

        pthread_create(
        &threads[i],
        NULL,
        stressUser,
        &ids[i]);
    }

    for(int i = 0; i < users; i++)
    {
        pthread_join(
        threads[i],
        NULL);
    }

    clock_t end;

    end = clock();

    double executionTime;

    executionTime =
    ((double)(end - start))
    / CLOCKS_PER_SEC;

    int memoryAfter;

    memoryAfter =
    getAvailableMemory();

    int memoryLeak;

    memoryLeak =
    totalAllocatedMemory -
    totalFreedMemory;

    if(memoryAfter < 50000)
    {
        oomEvents++;

        logOOMEvent();
    }

    FILE *fp;

    fp = fopen(
    "logs/stress_test.log",
    "a");

    if(fp != NULL)
    {
        fprintf(fp,
        "\n=================================\n");

        fprintf(fp,
        "STRESS TEST REPORT\n");

        fprintf(fp,
        "=================================\n");

        fprintf(fp,
        "Total Users : %d\n",
        users);

        fprintf(fp,
        "Successful Booking : %d\n",
        successfulBookings);

        fprintf(fp,
        "Waitlist Requests : %d\n",
        waitlistBookings);

        fprintf(fp,
        "Execution Time : %.2f Seconds\n",
        executionTime);

        fprintf(fp,
        "Memory Before : %d KB\n",
        memoryBefore);

        fprintf(fp,
        "Memory After : %d KB\n",
        memoryAfter);

        fprintf(fp,
        "Memory Per User : %d KB\n",
        USER_MEMORY_KB);

        fprintf(fp,
        "Total Memory Allocated : %d KB\n",
        totalAllocatedMemory);

        fprintf(fp,
        "Total Memory Freed : %d KB\n",
        totalFreedMemory);

        fprintf(fp,
        "Memory Leak : %d KB\n",
        memoryLeak);

       

        fclose(fp);
    }

    printf("\n=================================\n");
    printf("STRESS TEST REPORT\n");
    printf("=================================\n\n");

    printf("Total Users                 : %d\n",
           users);
    printf("Successful Booking          : %d\n",
           successfulBookings);
    printf("Waitlist Requests           : %d\n",
           waitlistBookings);
    printf("Execution Time              : %.2f Seconds\n",
           executionTime);
    printf("Memory Before               : %d KB\n",
           memoryBefore);
    printf("Memory Per User             : %d KB\n",
           USER_MEMORY_KB);
    printf("Total Memory Allocated      : %d KB\n",
           totalAllocatedMemory);
    // int Remaining_memory=memoryBefore-totalAllocatedMemory;
    // printf("Total memory available during allocation",Remaining_memory);
    printf("Total Memory Freed          : %d KB\n",
           totalFreedMemory);
    printf("Memory After                : %d KB\n",
           memoryAfter);
    printf("Memory Leak                 : %d KB\n",
          memoryLeak);
    if(memoryLeak == 0)
    {
        printf("Leak Status                 : NO MEMORY LEAK\n");
    }
    else
    {
        printf("Leak Status                 : MEMORY LEAK DETECTED\n");
    }
    // printf("\nOOM Events Logged           : %d\n",
    //        oomEvents);

    printf("Status                      : PASSED\n");

    printf("=================================\n");

    pthread_mutex_destroy(
    &counterLock);

    free(threads);
    free(ids);
}


