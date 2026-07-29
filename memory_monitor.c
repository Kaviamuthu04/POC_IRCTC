#include <stdio.h>
#include <string.h>

#include"memory_monitor.h"
void logOOMEvent()
{
    FILE *fp;

    fp = fopen("data/oom.log","a");

    if(fp == NULL)
    {
        return;
    }

    fprintf(fp,
    "WARNING : MEMORY PRESSURE DETECTED\n");

    fclose(fp);
}
int getAvailableMemory()
{
    FILE *fp;

    char line[256];
    int memAvailable = 0;

    fp = fopen("/proc/meminfo","r");

    if(fp == NULL)
        return 0;

    while(fgets(line,sizeof(line),fp))
    {
        if(strncmp(line,
                   "MemAvailable:",
                   13)==0)
        {
            sscanf(line,
                   "MemAvailable: %d",
                   &memAvailable);
            break;
        }
    }

    fclose(fp);

    return memAvailable;
}