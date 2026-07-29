#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#define MEMORY_NORMAL      100000
#define MEMORY_WARNING      50000
#define MEMORY_CRITICAL     20000
#include <stddef.h>
void *memoryAllocate(size_t size);
void memoryRelease(void *ptr);

int getAvailableMemory(void);

void logOOMEvent(void);

void addUserToQueue(char username[]);

int countOOMWarnings(void);
int countQueuedUsers(void);

void showOOMDashboard(void);

void runOOMAnalysis(void);

#endif
