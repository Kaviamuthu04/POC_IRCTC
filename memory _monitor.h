#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H


int getAvailableMemory();

void addUserToQueue(char username[]);

void logOOMEvent();

#endif