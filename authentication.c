#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "logger.h"

#define USER_FILE "data/users.dat"

typedef struct
{
    

    char username[50];
    char email[100];
    char phone[15];

    char password[50];

} User;

void viewLoginHistory()
{
    FILE *fp;
    char ch;

    fp = fopen("data/login_history.dat","r");

    if(fp == NULL)
    {
        printf("No Login History Found\n");
        return;
    }

    while((ch=fgetc(fp))!=EOF)
    {
        putchar(ch);
    }

    fclose(fp);
}

/* ---------------- REGISTER USER ---------------- */

void registerUser()
{
    User user;

    FILE *fp;

    fp = fopen(USER_FILE, "a");

    if(fp == NULL)
    {
        printf("Error Opening User File\n");
        return;
    }

    printf("\n===== USER REGISTRATION =====\n");

    

    printf("Username: ");
    scanf("%s",user.username);

    printf("Email: ");
    scanf("%s",user.email);

    printf("Phone: ");
    scanf("%s",user.phone);

    printf("Password: ");
    scanf("%s",user.password);

    fprintf(fp,
            "%s|%s|%s|%s\n",
            user.username,
            user.email,
            user.phone,
            user.password);

    fclose(fp);

    printf("\nRegistration Successful\n");
}

/* ---------------- LOGIN USER ---------------- */
int loginUser()
{
    char input[100];
    char password[50];

    User user;

    FILE *fp;
    FILE *logfp;

    int captcha;
    int userCaptcha;

    captcha = rand() % 9000 + 1000;

    printf("\n===== LOGIN =====\n");

    printf("Username / Email / Phone : ");
    scanf("%s", input);

    printf("Password : ");
    scanf("%s", password);

    printf("Captcha : %d\n", captcha);

    printf("Enter Captcha : ");
    scanf("%d", &userCaptcha);

    if(userCaptcha != captcha)
    {
        printf("Invalid Captcha\n");
        return 0;
    }

    fp = fopen(USER_FILE, "r");

    if(fp == NULL)
    {
        printf("User File Not Found\n");
        return 0;
    }

    while(fscanf(fp,
           "%49[^|]|%99[^|]|%14[^|]|%49[^\n]\n",
           user.username,
           user.email,
           user.phone,
           user.password) == 4)
    {
        if(
           (strcmp(input, user.username) == 0 ||
            strcmp(input, user.email) == 0 ||
            strcmp(input, user.phone) == 0)
            &&
           strcmp(password, user.password) == 0
          )
        {
            fclose(fp);

            printf("\nLogin Successful\n");
            printf("Welcome %s\n", user.username);
            logLogin(user.username);
            /* Store Login History */

            logfp = fopen(
                    "data/login_history.dat",
                    "a");

            if(logfp != NULL)
            {
                time_t now;
                time(&now);

                fprintf(logfp,
                "\n=====================\n");

                fprintf(logfp,
                "Username   : %s\n",
                user.username);

                fprintf(logfp,
                "Email      : %s\n",
                user.email);

                fprintf(logfp,
                "Phone      : %s\n",
                user.phone);

                fprintf(logfp,
                "Login Time : %s",
                ctime(&now));

                fprintf(logfp,
                "=====================\n");

                fclose(logfp);
            }

            return 1;
        }
    }

    fclose(fp);

    printf("\nInvalid Credentials\n");

    return 0;
}


void forgotPassword()
{
    char input[100];
    char newPassword[50];

    User user;

    FILE *fp;
    FILE *temp;

    int found = 0;

    fp = fopen(USER_FILE,"r");

    if(fp == NULL)
    {
        printf("User File Not Found\n");
        return;
    }

    temp = fopen("data/temp.dat","w");

    printf("\n===== FORGOT PASSWORD =====\n");

    printf("Enter Username / Email / Phone : ");
    scanf("%s",input);

    while(fscanf(fp,
                 "%49[^|]|%99[^|]|%14[^|]|%49[^\n]\n",
                
                 user.username,
                 user.email,
                 user.phone,
                 user.password)==4)

    {
        if(strcmp(input,user.username)==0 ||
        
           strcmp(input,user.email)==0 ||
           strcmp(input,user.phone)==0)
        {
            found = 1;

            printf("\nAccount Found\n");

            printf("Enter New Password: ");
            scanf("%s",newPassword);

            strcpy(user.password,newPassword);
        }

        fprintf(temp,
                "%s|%s|%s|%s\n",
                user.username,
                user.email,
                user.phone,
                user.password);
    }

    fclose(fp);
    fclose(temp);
    printf("Found = %d\n", found);
    remove(USER_FILE);

    rename("data/temp.dat",USER_FILE);

    if(found)
        printf("Password Updated Successfully\n");
    else
        printf("User Not Found\n");
}
