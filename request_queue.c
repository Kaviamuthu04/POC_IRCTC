#include <stdio.h>

void addUserToQueue(char username[])
{
    FILE *fp;

    fp = fopen("data/queue.dat","a");

    if(fp == NULL)
        return;

    fprintf(fp,"%s\n",username);

    fclose(fp);

    printf("Server Busy\n");
    printf("Added To Waiting Queue\n");
}