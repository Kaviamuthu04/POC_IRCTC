#include <stdio.h>
#include <time.h>

void writeLog(char message[])
{
    FILE *fp;

    fp = fopen("logs/booking.log","a");

    time_t t = time(NULL);

    fprintf(fp,
            "%s : %s\n",
            ctime(&t),
            message);

    fclose(fp);
}



void logCancellation(int pnr)
{
    FILE *fp;

    time_t now;

    time(&now);

    fp = fopen(
        "logs/booking.log",
        "a");

    if(fp == NULL)
        return;

    fprintf(fp,
            "[%s] PNR %d Cancelled\n",
            ctime(&now),
            pnr);

    fclose(fp);

    printf("Booking Log Updated\n");
}
void logBooking(int pnr)
{
    FILE *fp;

    fp = fopen("logs/booking.log","a");

    fprintf(fp,
    "Booking Created PNR %d\n",
    pnr);

    fclose(fp);
}
void logPayment(int pnr)
{
    FILE *fp;

    fp = fopen("logs/booking.log","a");

    if(fp == NULL)
        return;

    fprintf(fp,
            "Payment Success PNR %d\n",
            pnr);

    fclose(fp);
}

void logLogin(char username[])
{
    FILE *fp;

    fp = fopen("logs/booking.log","a");

    fprintf(fp,
    "Login Success %s\n",
    username);

    fclose(fp);
}