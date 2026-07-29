#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<pthread.h>
#include<windows.h>
#include<logger.h>
#include "memory_manager.h"
#define CRITICAL_MEMORY 30000
#define WARNING_MEMORY  50000

/* Function Prototypes */

int getAvailableMemory(void);
void *memoryAllocate(size_t size);
void memoryRelease(void *ptr);

void logOOMEvent(void);
void addUserToQueue(char username[]);

int countOOMWarnings(void);
int countQueuedUsers(void);

void showOOMDashboard(void);

/* Memory Allocation */

void *memoryAllocate(size_t size)
{
    void *ptr;

    ptr = malloc(size);

    if(ptr == NULL)
    {
        printf("Memory Allocation Failed\n");
    }

    return ptr;
}

/* Memory Release */

void memoryRelease(void *ptr)
{
    if(ptr != NULL)
    {
        free(ptr);
    }
}

/* Simulated Available Memory */

int getAvailableMemory(void)
{
    return 25000;
}

/* Log OOM Event */

void logOOMEvent(void)
{
    FILE *fp;

    fp = fopen("data/oom.log","a");

    if(fp == NULL)
    {
        return;
    }

    time_t t;

    t = time(NULL);

    fprintf(fp,
            "[%s] WARNING : MEMORY PRESSURE DETECTED\n",
            ctime(&t));

    fclose(fp);
}

/* Queue User */

void addUserToQueue(char username[])
{
    FILE *fp;

    fp = fopen("data/queue.dat","a");

    if(fp == NULL)
    {
        return;
    }

    fprintf(fp,
            "%s\n",
            username);

    fclose(fp);
}

/* Count Queue Users */

int countQueuedUsers(void)
{
    FILE *fp;

    char line[200];

    int count = 0;

    fp = fopen("data/queue.dat","r");

    if(fp == NULL)
    {
        return 0;
    }

    while(fgets(line,
                sizeof(line),
                fp) != NULL)
    {
        count++;
    }

    fclose(fp);

    return count;
}

/* Count OOM Warnings */

int countOOMWarnings(void)
{
    FILE *fp;

    char line[300];

    int count = 0;

    fp = fopen("data/oom.log","r");

    if(fp == NULL)
    {
        return 0;
    }

    while(fgets(line,
                sizeof(line),
                fp) != NULL)
    {
        if(strstr(line,
                  "WARNING : MEMORY PRESSURE DETECTED")
           != NULL)
        {
            count++;
        }
    }

    fclose(fp);

    return count;
}

/* OOM Dashboard */

void showOOMDashboard(void)
{
    int availableMemory;
    int queuedUsers;
    int oomWarnings;

    availableMemory =
    getAvailableMemory();

    queuedUsers =
    countQueuedUsers();

    oomWarnings =
    countOOMWarnings();

    printf("\n=================================\n");
    printf("          OOM DASHBOARD\n");
    printf("=================================\n");

    printf("Available Memory : %d KB\n",
           availableMemory);

    if(availableMemory < CRITICAL_MEMORY)
    {
        printf("Memory Status    : CRITICAL\n");
    }
    else if(availableMemory < WARNING_MEMORY)
    {
        printf("Memory Status    : WARNING\n");
    }
    else
    {
        printf("Memory Status    : HEALTHY\n");
    }

    printf("Queued Users     : %d\n",
           queuedUsers);

    printf("OOM Warnings     : %d\n",
           oomWarnings);

    printf("Load Shedding    : %s\n",
           availableMemory <
           CRITICAL_MEMORY ?
           "ACTIVE" :
           "INACTIVE");

    printf("=================================\n");
}
