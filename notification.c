#include <stdio.h>

void sendNotification()
{
    printf("\n");
    printf("==================================\n");
    printf("Booking Confirmed\n");
    printf("SMS Sent Successfully\n");
    printf("Email Sent Successfully\n");
    printf("==================================\n");
}
void sendPromotionNotification(int pnr)
{
    FILE *fp;

    fp = fopen("data/notifications.dat","a");

    if(fp == NULL)
        return;

    fprintf(fp,
    "PNR %d : Waitlist Promoted. Complete Payment To Confirm Seat.\n",
    pnr);

    fclose(fp);
}
void sendCancellationNotification(
        int pnr,
        float refund)
{
    FILE *fp;

    fp=fopen(
        "data/notifications.dat",
        "a");

    if(fp==NULL)
        return;

    fprintf(fp,
        "PNR:%d Refund:%.2f\n",
        pnr,
        refund);

    fclose(fp);

    printf("Notification Sent\n");
}
void viewNotifications()
{
    FILE *fp;
    char ch;

    fp = fopen("data/notifications.dat","r");

    if(fp == NULL)
    {
        printf("No Notifications Found\n");
        return;
    }

    while((ch=fgetc(fp))!=EOF)
    {
        putchar(ch);
    }

    fclose(fp);
}