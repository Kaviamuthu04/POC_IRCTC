#include <stdio.h>
#include <string.h>
#include"train.h"

void addTrain()
{
    Train t;

    FILE *fp;

    fp = fopen("data/trains.dat","a");

    if(fp == NULL)
    {
        printf("File Error\n");
        return;
    }

    printf("\n===== ADD TRAIN =====\n");

    printf("Train Number: ");
    scanf("%d",&t.trainNo);

    getchar();  // Clear newline

    printf("Train Name: ");
    scanf(" %[^\n]",t.trainName);

    printf("Source: ");
    scanf(" %[^\n]",t.source);

    printf("Destination: ");
    scanf(" %[^\n]",t.destination);

    printf("Departure Time: ");
    scanf(" %[^\n]",t.departure);

    printf("Arrival Time: ");
    scanf(" %[^\n]",t.arrival);

    printf("Platform Number: ");
    scanf("%d",&t.platform);

    printf("Total Seats: ");
    scanf("%d",&t.totalSeats);

    t.availableSeats = t.totalSeats;


    fprintf(fp,
    "%d|%s|%s|%s|%s|%s|%d|%d|%d\n",
    t.trainNo,
    t.trainName,
    t.source,
    t.destination,
    t.departure,
    t.arrival,
    t.platform,
    t.totalSeats,
    t.availableSeats);

    fclose(fp);

    printf("\nTrain Added Successfully\n");
}

void releaseSeat(int trainNo)
{
    FILE *fp;
    FILE *temp;

    int fileTrainNo;
    int platform;
    int totalSeats;
    int availableSeats;

    char trainName[50];
    char source[50];
    char destination[50];
    char departure[20];
    char arrival[20];

    fp = fopen("data/trains.dat","r");

    if(fp == NULL)
    {
        printf("Train File Not Found\n");
        return;
    }

    temp = fopen("data/train_temp.dat","w");

    if(temp == NULL)
    {
        fclose(fp);
        printf("Temp File Error\n");
        return;
    }

    while(fscanf(fp,
    "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%d|%d\n",
    &fileTrainNo,
    trainName,
    source,
    destination,
    departure,
    arrival,
    &platform,
    &totalSeats,
    &availableSeats) == 9)
    {
        if(fileTrainNo == trainNo)
        {
            availableSeats++;

            printf("\nSeat Released For Train %d\n",
                   trainNo);

            printf("Updated Available Seats : %d\n",
                   availableSeats);
        }

        fprintf(temp,
        "%d|%s|%s|%s|%s|%s|%d|%d|%d\n",
        fileTrainNo,
        trainName,
        source,
        destination,
        departure,
        arrival,
        platform,
        totalSeats,
        availableSeats);
    }

    fclose(fp);
    fclose(temp);

    remove("data/trains.dat");

    rename("data/train_temp.dat",
           "data/trains.dat");
}
// void releaseSeat(int trainNo)
// {
//     printf("Seat Released For Train %d\n",trainNo);
// }
void viewTrains()
{
    FILE *fp;

    Train t;

    char source[50];
    char destination[50];

    int found = 0;

    fp = fopen("data/trains.dat","r");

    if(fp == NULL)
    {
        printf("Train File Not Found\n");
        return;
    }

    printf("Source: ");
    scanf(" %[^\n]",source);

    printf("Destination: ");
    scanf(" %[^\n]",destination);

    while(fscanf(fp,
          "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%d|%d\n",
          &t.trainNo,
          t.trainName,
          t.source,
          t.destination,
          t.departure,
          t.arrival,
          &t.platform,
          &t.totalSeats,
          &t.availableSeats) == 9)
    {
        if(strcmp(source,t.source) == 0 &&
           strcmp(destination,t.destination) == 0)
        {
            found = 1;

            printf("\n========================\n");

            printf("Train Number     : %d\n",
                   t.trainNo);

            printf("Train Name       : %s\n",
                   t.trainName);

            printf("Source           : %s\n",
                   t.source);

            printf("Destination      : %s\n",
                   t.destination);

            printf("Departure Time   : %s\n",
                   t.departure);

            printf("Arrival Time     : %s\n",
                   t.arrival);

            printf("Platform Number  : %d\n",
                   t.platform);

            printf("Total Seats      : %d\n",
                   t.totalSeats);

            printf("Available Seats  : %d\n",
                   t.availableSeats);

            printf("========================\n");
        }
    }

    if(found == 0)
    {
        printf("\nNo Trains Available For This Route\n");
    }

    fclose(fp);
}

void decreaseSeats(int trainNo, int seatsBooked)
{
    FILE *fp;
    FILE *temp;

    Train t;

    fp = fopen("data/trains.dat","r");

    if(fp == NULL)
    {
        printf("Train File Not Found\n");
        return;
    }

    temp = fopen("data/temp.dat","w");

    while(fscanf(fp,
    "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d|%d|%d\n",
    &t.trainNo,
    t.trainName,
    t.source,
    t.destination,
    t.departure,
    t.arrival,
    &t.platform,
    &t.totalSeats,
    &t.availableSeats) == 9)
    {
        if(t.trainNo == trainNo)
        {
            t.availableSeats -= seatsBooked;

            if(t.availableSeats < 0)
            {
                t.availableSeats = 0;
            }
        }

        fprintf(temp,
        "%d|%s|%s|%s|%s|%s|%d|%d|%d\n",
        t.trainNo,
        t.trainName,
        t.source,
        t.destination,
        t.departure,
        t.arrival,
        t.platform,
        t.totalSeats,
        t.availableSeats);
    }

    fclose(fp);
    fclose(temp);

    remove("data/trains.dat");
    rename("data/temp.dat","data/trains.dat");
}

void updateTrainSchedule()
{
    FILE *fp;
    FILE *temp;

    Train train;

    int targetTrainNo;
    int newPlatform;

    char newDeparture[20];
    char newArrival[20];

    int found = 0;

    fp = fopen("data/trains.dat","r");

    if(fp == NULL)
    {
        printf("Train File Not Found\n");
        return;
    }

    temp = fopen("data/temp.dat","w");

    if(temp == NULL)
    {
        fclose(fp);
        printf("Temp File Error\n");
        return;
    }

    printf("\n===== UPDATE TRAIN SCHEDULE =====\n");

    printf("Enter Train Number: ");
    scanf("%d",&targetTrainNo);

    printf("New Departure Time: ");
    scanf("%s",newDeparture);

    printf("New Arrival Time: ");
    scanf("%s",newArrival);

    printf("New Platform Number: ");
    scanf("%d",&newPlatform);

    while(fscanf(fp,
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
        if(train.trainNo == targetTrainNo)
        {
            strcpy(train.departure,newDeparture);
            strcpy(train.arrival,newArrival);
            train.platform = newPlatform;

            found = 1;
        }

        fprintf(temp,
        "%d|%s|%s|%s|%s|%s|%d|%d|%d\n",
        train.trainNo,
        train.trainName,
        train.source,
        train.destination,
        train.departure,
        train.arrival,
        train.platform,
        train.totalSeats,
        train.availableSeats);
    }

    fclose(fp);
    fclose(temp);

    remove("data/trains.dat");

    rename("data/temp.dat",
           "data/trains.dat");

    if(found)
    {
        printf("Schedule Updated Successfully\n");
    }
    else
    {
        printf("Train Not Found\n");
    }
}
