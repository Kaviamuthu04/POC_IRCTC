#include <pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "memory_manager.h"
#include "logger.h"
#define CRITICAL_MEMORY 30000
#define WARNING_MEMORY 50000

void *loadGenerator(void *arg)
{
    int checksPerformed = 0;

    for(checksPerformed = 0;
        checksPerformed < 10;
        checksPerformed++)
    {
        char *buffer;

        buffer = (char *)malloc(1024);

        if(buffer != NULL)
        {
            memset(buffer, 0, 1024);

            free(buffer);
        }

        Sleep(500);
    }

    return NULL;
}
void *memoryMonitor(void *arg)
{
    for(int i=0;i<10;i++)
    {
        int freeMemory;

        freeMemory =
        getAvailableMemory();

        printf("\nAvailable Memory : %d KB\n",
               freeMemory);

        Sleep(1000);
    }

    return NULL;
}
void *oomLogger(void *arg)
{
    int checksPerformed = 0;

    for(checksPerformed = 0;
        checksPerformed < 10;
        checksPerformed++)
    {
        int memory;

        memory = getAvailableMemory();

        if(memory < 50000)
        {
            logOOMEvent();
        }

        Sleep(500);
    }

    return NULL;
}

void runOOMAnalysis(void)
{
    int availableMemory;
    int warningCount = 0;

    pthread_t monitorThread;
    pthread_t loggerThread;
    pthread_t loadThread;

    printf("\n=================================\n");
    printf("        OOM ANALYSIS\n");
    printf("=================================\n");

    availableMemory =
    getAvailableMemory();

    printf("Available Memory : %d KB\n",
           availableMemory);

    /* Memory Status */

    if(availableMemory < CRITICAL_MEMORY)
    {
        printf("Memory Status    : CRITICAL\n");

        logOOMEvent();

        addUserToQueue(
        "BOOKING_REQUEST");

        printf("Action Taken     : USER QUEUED\n");
        printf("Load Shedding    : ACTIVE\n");

        warningCount++;
    }
    else if(availableMemory < WARNING_MEMORY)
    {
        printf("Memory Status    : WARNING\n");

        logOOMEvent();

        printf("Action Taken     : MONITORING\n");
        printf("Load Shedding    : INACTIVE\n");

        warningCount++;
    }
    else
    {
        printf("Memory Status    : HEALTHY\n");
        printf("Action Taken     : NONE\n");
        printf("Load Shedding    : INACTIVE\n");
    }

    printf("\nStarting OOM Monitoring Threads...\n");

    pthread_create(
        &monitorThread,
        NULL,
        memoryMonitor,
        NULL);

    pthread_create(
        &loggerThread,
        NULL,
        oomLogger,
        NULL);

    pthread_create(
        &loadThread,
        NULL,
        loadGenerator,
        NULL);

    pthread_join(
        monitorThread,
        NULL);

    pthread_join(
        loggerThread,
        NULL);

    pthread_join(
        loadThread,
        NULL);

    printf("\n=================================\n");
    printf("      OOM ANALYSIS REPORT\n");
    printf("=================================\n");

    printf("Available Memory : %d KB\n",
           availableMemory);

    printf("Memory Status    : %s\n",
           availableMemory < CRITICAL_MEMORY ?
           "CRITICAL" :
           (availableMemory < WARNING_MEMORY ?
           "WARNING" :
           "HEALTHY"));

    printf("Queued Users     : %d\n",
           countQueuedUsers());

    printf("OOM Warnings     : %d\n",
           countOOMWarnings());

    
    //printf("Checks Performed : d\n",n);


    printf("Load Shedding    : %s\n",
           availableMemory < CRITICAL_MEMORY ?
           "ACTIVE" :
           "INACTIVE");

    printf("Status           : ANALYSIS COMPLETED\n");

    printf("=================================\n");
}