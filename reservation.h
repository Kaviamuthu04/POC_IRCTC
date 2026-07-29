
#ifndef RESERVATION_H
#define RESERVATION_H
typedef struct
{
    int pnr;

    int userId;

    char PassengerName[50];

    char source[50];
    char destination[50];

    char journeyDate[20];

    char departureTime[20];
    char arrivalTime[20];

    char bookingStatus[20];

    float fare;

} Reservation;

    int generatePNR(void);
    
    void bookTicket();
    
    void viewReservation();
    void cancelTicket();
    void promoteWaitlistPassenger(int cancelledTrainNo);
    void partialCancellation();
    int seatExists(char seatNo[]);
    void allocateSeat(char seatNo[]);
    int getAvailableMemory();
    void logOOMEvent();
    void releaseSeat();
    int seatExists(char seatNo[]);
    void updateReservationFile(int cancelledPNR);
    
    int generatePNR();

#endif
