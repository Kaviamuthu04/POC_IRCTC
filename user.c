#include <stdio.h>

void viewUsers()
{
    FILE *fp;
    char ch;

    fp = fopen("data/users.dat","r");

    if(fp == NULL)
    {
        printf("No Users Found\n");
        return;
    }

    printf("\n===== USERS =====\n");

    while((ch=fgetc(fp))!=EOF)
    {
        putchar(ch);
    }

    fclose(fp);
}