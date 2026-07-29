
#ifndef TRAIN_H
#define TRAIN_H
typedef struct
{
    int trainNo;
    char trainName[50];
    char source[50];
    char destination[50];
    char departure[20];
    char arrival[20];
    int platform;
    int totalSeats;
    int availableSeats;
} Train;

void addTrain();

void viewTrains();

void updateTrainSchedule();

void decreaseSeats(
     int trainNo,
     int seatsBooked);

void releaseSeat(
     int trainNo);

#endif