#ifndef USER_H
#define USER_H

typedef struct
{
    int userId;
    char userName[50];
    char password[50];
    char phone[15];
    char email[100];
} User;

void viewUsers();

#endif