#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "payment.h"
#include "waitlist.h"
#include "notification.h"
#include "logger.h"
#include "train.h"
#include "reservation.h"
#include <string.h>
#include "memory_monitor.h"

float calculateRefund(float fare);
void updatePaymentStatus(int pnr);
void promoteRAC();
void promoteWaitlist();
void sendCancellationNotification(int pnr,float refund);
void logCancellation(int pnr);
void allocateSeat(char seatNo[]);
void releaseSeat();
int getAvailableMemory();
void logOOMEvent();
void addUserToQueue(char username[]);
int getAvailableMemory();

typedef struct
{
    char name[50];
    int age;
    char gender[20];
    char category[20];
    char seatNo[20];

} Passenger;
int isJourneyExpired(char travelDate[])
{
    int day;
    int month;
    int year;

    sscanf(travelDate,
           "%d-%d-%d",
           &day,
           &month,
           &year);

    time_t t;
    struct tm *current;

    t = time(NULL);
    current = localtime(&t);

    int currentDay = current->tm_mday;
    int currentMonth = current->tm_mon + 1;
    int currentYear = current->tm_year + 1900;

    if(year < currentYear)
        return 1;

    if(year == currentYear &&
       month < currentMonth)
        return 1;

    if(year == currentYear &&
       month == currentMonth &&
       day < currentDay)
        return 1;

    return 0;
}
int generatePNR()
{
    FILE *fp;
    int lastPNR = 100000;

    fp = fopen("data/pnr.dat","r");

    if(fp != NULL)
    {
        fscanf(fp,"%d",&lastPNR);
        fclose(fp);
    }

    lastPNR++;

    fp = fopen("data/pnr.dat","w");

    if(fp != NULL)
    {
        fprintf(fp,"%d",lastPNR);
        fclose(fp);
    }

    return lastPNR;
}

int seatExists(char seatNo[])
{
    FILE *fp;

    char line[500];
    char existingSeat[20];

    fp = fopen("data/reservations.dat","r");

    if(fp == NULL)
    {
        return 0;
    }

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,
        "%*d|%*[^|]|%*d|%*[^|]|%*[^|]|%*d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*d|%*[^|]|%19[^|]",
        existingSeat);

        if(strcmp(existingSeat,seatNo) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;

}

void bookTicket()
{
    int pnr;
    int passengerCount;
    int bookedSeats = 0;
    int confirmedCount = 0;
    int waitlistCount = 0;

    float totalFare = 0;

    char source[50];
    char destination[50];
    char journeyDate[20];

    Train train;

    FILE *fp;

    fp = fopen("data/reservations.dat","a");

    if(fp == NULL)
    {
        printf("Reservation File Error\n");
        return;
    }

    printf("\n===== BOOK TICKET =====\n");

    printf("Source : ");
    scanf(" %[^\n]",source);

    printf("Destination : ");
    scanf(" %[^\n]",destination);

    printf("Journey Date (DD-MM-YYYY) : ");
    scanf("%s",journeyDate);

    FILE *trainfp;

    trainfp = fopen("data/trains.dat","r");

    if(trainfp == NULL)
    {
        printf("Train File Error\n");

        fclose(fp);
        return;
    }

    int foundTrain = 0;

    while(fscanf(trainfp,
    "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%d|%d\n",
    &train.trainNo,
    train.trainName,
    train.source,
    train.destination,
    train.departure,
    train.arrival,
    &train.platform,
    &train.totalSeats,
    &train.availableSeats) == 9)
    {
        if(strcmp(source,train.source) == 0 &&
           strcmp(destination,train.destination) == 0)
        {
            foundTrain = 1;
            break;
        }
    }

    fclose(trainfp);

    if(!foundTrain)
    {
        printf("No Train Available\n");

        fclose(fp);
        return;
    }

    printf("\nTrain No      : %d\n",
           train.trainNo);

    printf("Train Name    : %s\n",
           train.trainName);

    printf("AvailableSeat : %d\n",
           train.availableSeats);

    printf("\nNumber Of Passengers : ");
    scanf("%d",&passengerCount);

    /* CASE 2 */

    if(train.availableSeats > 0 &&
       passengerCount > train.availableSeats)
    {
        printf("\n=================================\n");
        printf("BOOKING FAILED\n");
        printf("Requested Seats : %d\n",
               passengerCount);

        printf("Available Seats : %d\n",
               train.availableSeats);

        printf("Only %d Seats Available\n",
               train.availableSeats);

        printf("=================================\n");

        fclose(fp);
        return;
    }

    Passenger *passengers;

    passengers =
    (Passenger *)malloc(
    sizeof(Passenger) * passengerCount);

    if(passengers == NULL)
    {
        printf("Memory Allocation Failed\n");

        fclose(fp);
        return;
    }

    pnr = generatePNR();

    printf("\nGenerated PNR : %d\n",
           pnr);

    for(int i=0;i<passengerCount;i++)
    {
        printf("\n====================\n");
        printf("Passenger %d\n",
               i + 1);
        printf("====================\n");

        printf("Name : ");
        scanf(" %[^\n]",
              passengers[i].name);

        printf("Age : ");
        scanf("%d",
              &passengers[i].age);

        printf("Gender : ");
        scanf("%s",
              passengers[i].gender);

        if(passengers[i].age <= 10)
            strcpy(passengers[i].category,
                   "Child");
        else
            strcpy(passengers[i].category,
                   "Adult");

        /* CASE 3 */

        if(train.availableSeats == 0)
        {
            FILE *wf;

            wf = fopen("data/waitlist.dat","a");

            if(wf == NULL)
            {
                printf("Waitlist File Error\n");
                continue;
            }

            fprintf(wf,
            "%d|%s|%d|%s|%s|%d|%s|%s|%s|%s|%s|%d|%s|WAITLIST\n",
            pnr,
            passengers[i].name,
            passengers[i].age,
            passengers[i].gender,
            passengers[i].category,
            train.trainNo,
            train.trainName,
            source,
            destination,
            train.departure,
            train.arrival,
            train.platform,
            journeyDate);

            fclose(wf);

            waitlistCount++;

            printf("%s Added To WAITLIST\n",
                   passengers[i].name);

            continue;
        }

        /* CASE 1 */

        allocateSeat(
            passengers[i].seatNo);

        totalFare +=
        calculateFare(
            passengers[i].age);

        fprintf(fp,
        "%d|%s|%d|%s|%s|%d|%s|%s|%s|%s|%s|%d|%s|%s|PAYMENT_PENDING\n",
        pnr,
        passengers[i].name,
        passengers[i].age,
        passengers[i].gender,
        passengers[i].category,
        train.trainNo,
        train.trainName,
        source,
        destination,
        train.departure,
        train.arrival,
        train.platform,
        journeyDate,
        passengers[i].seatNo);

        train.availableSeats--;

        bookedSeats++;
        confirmedCount++;
    }

    fclose(fp);

    if(bookedSeats > 0)
    {
        decreaseSeats(
            train.trainNo,
            bookedSeats);
    }

    printf("\n=================================\n");
    printf("BOOKING SUMMARY\n");
    printf("=================================\n");

    printf("PNR Number : %d\n",
           pnr);

    printf("Confirmed Passengers : %d\n",
           confirmedCount);

    printf("Waitlist Passengers : %d\n",
           waitlistCount);

    printf("Total Fare : %.2f\n",
           totalFare);

    printf("=================================\n");

    logBooking(pnr);

    free(passengers);
}


void viewReservation()
{
    FILE *fp;
    char ch;

    fp=fopen("data/reservations.dat","r");

    if(fp==NULL)
    {
        printf("No Reservations Found\n");
        return;
    }

    printf("\n===== RESERVATIONS =====\n");

    while((ch=fgetc(fp))!=EOF)
    {
        putchar(ch);
    }

    fclose(fp);
}
void partialCancellation()
{
    int pnr;
    int passengerNo;
    int cancelledTrainNo=0;
    FILE *fp;
    FILE *temp;

    char line[500];

    int count = 0;

    printf("Enter PNR : ");
    scanf("%d",&pnr);

    printf("Passenger Number To Cancel : ");
    scanf("%d",&passengerNo);

    fp = fopen("data/reservations.dat","r");

    if(fp == NULL)
    {
        printf("Reservation File Error\n");
        return;
    }

    temp = fopen("data/temp.dat","w");

    while(fgets(line,sizeof(line),fp))
    {
        int filePNR;

        sscanf(line,"%d|",&filePNR);

        if(filePNR == pnr)
        {
            count++;
            if(count == passengerNo)
    {
        sscanf(line,
        "%*d|%*[^|]|%*d|%*[^|]|%*[^|]|%d|",
        &cancelledTrainNo);

        printf("Cancelled Train = %d\n",
            cancelledTrainNo);

        printf("Passenger Cancelled Successfully\n");

        continue;
    }
        }

        fputs(line,temp);
    }

    fclose(fp);
    fclose(temp);

    remove("data/reservations.dat");
    rename("data/temp.dat","data/reservations.dat");

   releaseSeat(cancelledTrainNo);

promoteWaitlistPassenger(cancelledTrainNo);
    printf("Seat Released\n");
}

void cancelTicket()
{
    int pnr;
    int found = 0;
    int cancelled=0;

    char line[500];
    char choice;

    FILE *fp;
    FILE *temp;

    float fare = 450.0;
    float refund;

    printf("\n===== CANCEL TICKET =====\n");

    printf("Enter PNR Number: ");
    scanf("%d", &pnr);

    fp = fopen("data/reservations.dat", "r");

    if(fp == NULL)
    {
        printf("Reservation File Not Found\n");
        return;
    }

    temp = fopen("data/temp.dat", "w");

    if(temp == NULL)
    {
        printf("Temp File Error\n");
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp))
    {
        int filePNR;
        int trainNo;

        char journeyDate[20];
    
        char name[50];
        char gender[20];
        char category[20];
        char trainName[50];
        char source[50];
        char destination[50];
        char departure[20];
        char arrival[20];
        char seatNo[20];
        char status[20];
        int age;
        
        int platform;
        
        sscanf(line,
        "%d|%49[^|]|%d|%19[^|]|%19[^|]|%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%19[^|]|%19[^|]|%19[^|\n]",
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
        sscanf(line, "%d|", &filePNR);
        // printf("Journey Date = %s\n", journeyDate);
        if(filePNR == pnr)
        {
            found = 1;

            if(isJourneyExpired(journeyDate))
            {
                printf("\n=================================\n");
                printf("CANCELLATION NOT ALLOWED\n");
                printf("Journey Date : %s\n",
                       journeyDate);
                printf("Journey Already Completed\n");
                printf("Please Contact Administrator\n");
                printf("=================================\n");

                fputs(line, temp);

                continue;
            }

            printf("\nTicket Found\n");
            printf("%s\n", line);

            printf("\nAre You Sure You Want To Cancel ? (Y/N) : ");
            scanf(" %c", &choice);
            if(choice == 'Y' || choice == 'y')
            {
                cancelled = 1;

                refund = calculateRefund(fare);

                sscanf(line,
                    "%*d|%*[^|]|%*d|%*[^|]|%*[^|]|%d|",
                    &trainNo);

                releaseSeat(trainNo);

                promoteWaitlistPassenger(trainNo);

                updatePaymentStatus(pnr);

                sendCancellationNotification(
                    pnr,
                    refund);

                logCancellation(pnr);

                printf("\nRefund Amount : %.2f\n",
                    refund);

                printf("Ticket Cancelled Successfully\n");

                continue;
            }

            else
            {
                printf("Cancellation Aborted\n");

                fputs(line, temp);

                continue;
            }
        }

        fputs(line, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("data/reservations.dat");

    rename("data/temp.dat",
           "data/reservations.dat");


void updateReservationFile(
    int cancelledPNR)
{
    FILE *fp;
    FILE *temp;

    char line[500];

    int pnr;

    fp = fopen(
        "data/reservations.dat",
        "r");

    temp = fopen(
        "data/temp.dat",
        "w");

    while(fgets(line,
                sizeof(line),
                fp))
    {
        sscanf(line,"%d|",&pnr);

        if(pnr != cancelledPNR)
        {
            fputs(line,temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(
    "data/reservations.dat");

    rename(
    "data/temp.dat",
    "data/reservations.dat");
}