#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include"waitlist.h"
#include "reservation.h"
void sendPromotionNotification(int pnr);
void allocateSeat(char seatNo[])
{
    int coach;
    int seat;

    do
    {
        coach = rand()%10 + 1;
        seat = rand()%72 + 1;

        sprintf(seatNo,
                "S%d-%d",
                coach,
                seat);

    }while(seatExists(seatNo));
}
void promoteWaitlistPassenger(int cancelledTrainNo)
{
    FILE *wf;
    FILE *temp;
    FILE *rf;

    char firstRecord[500]="";
    char line[500];

    int found = 0;
   

    wf = fopen("data/waitlist.dat","r");

    if(wf == NULL)
    {
        printf("Waitlist Empty\n");
        return;
    }

   // temp = fopen("data/temp.dat","w");
temp = fopen("data/waitlist_temp.dat","w");
if(temp == NULL)
{
    fclose(wf);
    printf("Temp File Error\n");
    return;
}

while(fgets(line,sizeof(line),wf))
{
    int waitlistTrainNo;

    sscanf(line,
           "%*d|%*[^|]|%*d|%*[^|]|%*[^|]|%d|",
           &waitlistTrainNo);
    printf("\n--------------------------------------\n");
    printf("Record = %s", line);
    printf("Cancelled Train = %d\n", cancelledTrainNo);
    printf("Waitlist Train = %d\n", waitlistTrainNo);
    printf("\n--------------------------------------\n");    
    if(found == 0 &&
       waitlistTrainNo == cancelledTrainNo)
    {
        printf("REMOVING FROM WAITLIST : %s\n", line);

        strcpy(firstRecord,line);

        found = 1;

        continue;
    }

    fputs(line,temp);
}

fclose(wf);
fclose(temp);

if(found)
{   printf("Found = %d\n", found);
    remove("data/waitlist.dat");
    rename("data/waitlist_temp.dat","data/waitlist.dat");
    //rename("data/temp.dat","data/waitlist.dat");
}
else
{   remove("data/waitlist_temp.dat");
    //remove("data/temp.dat");

    printf("No Matching Waitlist Passenger Found\n");
    return;
}

    if(found)
{
    char seatNo[20];

    int pnr;

    float fare = 450.0;

    allocateSeat(seatNo);

    firstRecord[strcspn(firstRecord,"\n")] = '\0';

    sscanf(firstRecord,
           "%d|",
           &pnr);

    rf = fopen("data/reservations.dat","a");

    char *pos;

    pos = strstr(firstRecord,"WAITLIST");

    if(pos != NULL)
    {
        *pos = '\0';
    }

    fprintf(rf,
            "%s%s|PAYMENT_PENDING\n",
            firstRecord,
            seatNo);

    fclose(rf);

    sendPromotionNotification(pnr);

    printf("\n=================================\n");
    printf("WAITLIST PASSENGER PROMOTED\n");
    printf("=================================\n");
    printf("PNR Number  : %d\n", pnr);
    printf("Seat Number : %s\n", seatNo);
    printf("Fare Amount : %.2f\n", fare);
    printf("Status      : PAYMENT_PENDING\n");
    printf("Please Complete Payment\n");
    printf("=================================\n");
}
}
void addToWaitlist(char name[])
{
    FILE *fp;

    fp = fopen("data/waitlist.dat","a");
    
    if(fp == NULL)
        {
            printf("Waitlist File Error\n");
            return;
        }

    fprintf(fp,"%s\n",name);

    fclose(fp);

    printf("Added To Waitlist\n");
}
// void promoteRAC()
// {
//     printf("RAC Passenger Promoted\n");
// }
void promoteRAC()
{
    FILE *fp;

    char name[50];

    fp = fopen("data/rac.dat","r");

    if(fp == NULL)
        return;

    if(fgets(name,sizeof(name),fp))
    {
        printf("%s Promoted To CONFIRMED\n",
               name);
    }

    fclose(fp);
}
// void promoteWaitlist()
// {
//     printf("Waitlist Passenger Moved To RAC\n");
// }
void promoteWaitlist()
{
    FILE *fp;

    char name[50];

    fp = fopen("data/waitlist.dat","r");

    if(fp == NULL)
        return;

    if(fgets(name,sizeof(name),fp))
    {
        printf("%s Promoted To RAC\n",
               name);
    }

    fclose(fp);
}
void viewWaitlist()
{
    FILE *fp;

    char line[100];

    fp = fopen("data/waitlist.dat","r");

    if(fp == NULL)
    {
        printf("No Waitlist Data Found\n");
        return;
    }

    printf("\n===== WAITLIST =====\n");

    while(fgets(line,sizeof(line),fp))
    {
        printf("%s",line);
    }

    fclose(fp);
}