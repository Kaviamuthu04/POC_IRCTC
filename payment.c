#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "logger.h"

void confirmReservation(int pnr)
{
    FILE *fp;
    FILE *temp;

    char line[500];
    int filePNR;

    fp = fopen("data/reservations.dat","r");

    if(fp == NULL)
    {
        return;
    }

    temp = fopen("data/temp.dat","w");

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,"%d|",&filePNR);

        if(filePNR == pnr)
        {
            char *pos;

            pos = strstr(line,"PAYMENT_PENDING");

            if(pos != NULL)
            {
                *pos = '\0';

                fprintf(temp,
                "%sCONFIRMED\n",
                line);
            }
            else
            {
                fputs(line,temp);
            }
        }
        else
        {
            fputs(line,temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/reservations.dat");

    rename("data/temp.dat",
           "data/reservations.dat");
}
float calculateFare(int age)
{
    if(age <= 10)
    {
        return 225.0;
    }

    return 450.0;
}

float calculateRefund(float fare)
{
    float cancellationCharge = 50;

    float refund =
        fare - cancellationCharge;

    if(refund < 0)
        refund = 0;

    return refund;
}
typedef struct
{
    char name[50];
    int age;
    char gender[20];
    char category[20];
    char seatNo[20];
    int trainNo;
    char trainName[50];
    char departure[20];
    char arrival[20];
    int platform;

} Passenger;

void updatePaymentStatus(int pnr)
{
    FILE *fp;
    FILE *temp;

    char line[500];

    int filePNR;

    fp = fopen("data/payments.dat","r");

    if(fp == NULL)
    {
        return;
    }

    temp = fopen("data/payment_temp.dat","w");

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,"%d|",&filePNR);

        if(filePNR == pnr)
        {
            char paymentMethod[50];
            float amount;

            sscanf(line,
                   "%d|%f|%[^|]",
                   &filePNR,
                   &amount,
                   paymentMethod);

            fprintf(temp,
                    "%d|%.2f|%s|REFUNDED\n",
                    filePNR,
                    amount,
                    paymentMethod);
        }
        else
        {
            fputs(line,temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/payments.dat");

    rename("data/payment_temp.dat",
           "data/payments.dat");
}
void viewPaymentStatus()
{
    FILE *fp;
    char line[500];

    fp = fopen("data/payments.dat","r");

    if(fp == NULL)
    {
        printf("No Payment Records Found\n");
        return;
    }

    printf("\n===== PAYMENT STATUS =====\n");

    while(fgets(line,sizeof(line),fp))
    {
        printf("%s",line);
    }

    fclose(fp);
}
void makePayment()
{
    int pnr;
    int found = 0;
    int option;

    FILE *fp;
    FILE *temp;
    FILE *payfp;

    int adultCount = 0;
    int childCount = 0;

    float totalAmount = 0;

    char line[500];

    printf("\n===== PAYMENT MODULE =====\n");

    printf("Enter PNR Number : ");
    scanf("%d",&pnr);

    fp = fopen("data/reservations.dat","r");

    if(fp == NULL)
    {
        printf("Reservation File Not Found\n");
        return;
    }

    temp = fopen("data/temp.dat","w");

    if(temp == NULL)
    {
        fclose(fp);
        return;
    }

    printf("\nPayment Methods\n");
    printf("1. UPI\n");
    printf("2. GPay\n");
    printf("3. PhonePe\n");
    printf("4. Paytm\n");

    printf("Select Option : ");
    scanf("%d",&option);

    while(fgets(line,sizeof(line),fp))
    {
        int filePNR;

        char name[50];
        int age;

        char gender[20];
        char category[20];

        int trainNo;
        int platform;

        char trainName[50];
        char source[50];
        char destination[50];

        char departure[20];
        char arrival[20];

        char journeyDate[20];
        char seatNo[20];
        char status[30];
        int fields;
        fields=sscanf(line,
        "%d|%49[^|]|%d|%19[^|]|%19[^|]|%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%19[^|]|%19[^|]|%29[^\n]",
        &filePNR,
        name,
        &age,
        gender,
        category,
        &trainNo,
        trainName,
        source,
        destination,
        departure,
        arrival,
        &platform,
        journeyDate,
        seatNo,
        status);
        if(fields!=15)
        {
            fputs(line,temp);
            continue;
        }


        if(filePNR == pnr)
            {
                found = 1;

                if(strcmp(status,"PAYMENT_PENDING") != 0)
                {
                    printf("\nPayment Not Required\n");
                    printf("Current Status : %s\n",status);

                    fputs(line,temp);
                    continue;
                }

                strcpy(status,"CONFIRMED");

                if(age <= 10)
                {
                    childCount++;
                    totalAmount += 225.0;
                }
                else
                {
                    adultCount++;
                    totalAmount += 450.0;
                }

                fprintf(temp,
                "%d|%s|%d|%s|%s|%d|%s|%s|%s|%s|%s|%d|%s|%s|CONFIRMED\n",
                filePNR,
                name,
                age,
                gender,
                category,
                trainNo,
                trainName,
                source,
                destination,
                departure,
                arrival,
                platform,
                journeyDate,
                seatNo);

                printf("\n=================================\n");
                printf("PAYMENT SUCCESSFUL\n");
                printf("PNR Number : %d\n",filePNR);
                printf("Amount     : %.2f\n",
                    (age <= 10) ? 225.0 : 450.0);
                printf("Seat No    : %s\n",seatNo);
                printf("Status     : CONFIRMED\n");
                printf("=================================\n");
            

            printf("\n=================================\n");
            printf("           E-TICKET\n");
            printf("=================================\n");

            printf("PNR Number      : %d\n",filePNR);
            printf("Train Number    : %d\n",trainNo);
            printf("Train Name      : %s\n",trainName);
            printf("Passenger Name  : %s\n",name);
            printf("Age             : %d\n",age);
            printf("Gender          : %s\n",gender);
            printf("Category        : %s\n",category);
            printf("Source          : %s\n",source);
            printf("Destination     : %s\n",destination);
            printf("Departure Time  : %s\n",departure);
            printf("Arrival Time    : %s\n",arrival);
            printf("Platform Number : %d\n",platform);
            printf("Journey Date    : %s\n",journeyDate);
            printf("Seat Number     : %s\n",seatNo);
            printf("Ticket Status   : CONFIRMED\n");

            printf("=================================\n");

            FILE *notify;

            notify = fopen("data/notifications.dat","a");

            if(notify != NULL)
            {
                fprintf(notify,
                "\n=================================\n");

                fprintf(notify,
                "PNR Number      : %d\n",
                filePNR);

                fprintf(notify,
                "Train Number    : %d\n",
                trainNo);

                fprintf(notify,
                "Train Name      : %s\n",
                trainName);

                fprintf(notify,
                "Passenger Name  : %s\n",
                name);

                fprintf(notify,
                "Source          : %s\n",
                source);

                fprintf(notify,
                "Destination     : %s\n",
                destination);

                fprintf(notify,
                "Journey Date    : %s\n",
                journeyDate);

                fprintf(notify,
                "Seat Number     : %s\n",
                seatNo);

                fprintf(notify,
                "Status          : CONFIRMED\n");

                fprintf(notify,
                "=================================\n");

                fclose(notify);
            }
        }
        else
        {
            fputs(line,temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/reservations.dat");

    rename("data/temp.dat",
           "data/reservations.dat");

    if(found == 0)
    {
        printf("\nPNR Not Found\n");
        return;
    }

    payfp = fopen("data/payments.dat","a");

    if(payfp != NULL)
    {
        fprintf(payfp,
        "PNR:%d\n",
        pnr);

        fprintf(payfp,
        "Passengers:%d\n",
        adultCount + childCount);

        fprintf(payfp,
        "Adult Count:%d\n",
        adultCount);

        fprintf(payfp,
        "Child Count:%d\n",
        childCount);

        fprintf(payfp,
        "Amount:%.2f\n",
        totalAmount);

        switch(option)
        {
            case 1:
                fprintf(payfp,"Mode:UPI\n");
                break;

            case 2:
                fprintf(payfp,"Mode:GPay\n");
                break;

            case 3:
                fprintf(payfp,"Mode:PhonePe\n");
                break;

            case 4:
                fprintf(payfp,"Mode:Paytm\n");
                break;
        }

        fprintf(payfp,
        "Status:SUCCESS\n");

        fprintf(payfp,
        "----------------------\n");

        fclose(payfp);
    }

 
        printf("\n=================================\n");
        printf("PAYMENT SUCCESSFUL\n");
        printf("PNR : %d\n",pnr);
        printf("Reservation Status Updated\n");
        printf("Current Status : CONFIRMED\n");
        printf("=================================\n");
    logPayment(pnr);
}
