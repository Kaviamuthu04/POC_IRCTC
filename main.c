#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "waitlist.h"
#include "authentication.h"
#include "train.h"
#include "notification.h"
#include "reservation.h"
#include "payment.h"
#include "report.h"
#include "stress_test.h"
#include "memory_manager.h"
#include "oom_analysis.h"

int getAvailableMemory();
void logOOMEvent();
void addUserToQueue(char username[]);

void initializeFiles()
{   
    srand(time(NULL));
    FILE *fp;

    fp = fopen("data/users.dat","a");
    if(fp) fclose(fp);

    fp = fopen("data/trains.dat","a");
    if(fp) fclose(fp);

    fp = fopen("data/reservations.dat","a");
    if(fp) fclose(fp);

    fp = fopen("data/payments.dat","a");
    if(fp) fclose(fp);

    fp = fopen("data/login_history.dat","a");
    if(fp) fclose(fp);

    fp = fopen("data/notifications.dat","a");
    if(fp) fclose(fp);

    printf("System Initialized Successfully!\n");
}

int main()
{
    initializeFiles();
   
   
    int choice;
   
    while(1)
    {
        printf("\n=================================\n");
        printf("      IRCTC TATKAL SYSTEM\n");
        printf("=================================\n");

        printf("1. Register User\n");
        printf("2. Login\n");
        printf("3. Add Train\n");
        printf("4. View Trains\n");
        printf("5. Book Ticket\n");
        printf("6. Cancel Ticket\n");
        printf("7. View Reservation\n");
        printf("8. Payment\n");
        printf("9. Generate Report\n");
        printf("10. Stress Test\n");
        printf("11. Forgot Password\n");
        printf("12. Update train schedule\n");
        printf("13. View Waitlist\n");
        printf("14. Revenue Report\n");
        printf("15. View Login History\n");
        printf("16. View Notifications\n");
        printf("17. Partial Cancellation\n");
        printf("18. OOM Analysis\n");
        printf("19. OOM Dashboard\n");
        printf("20. OOMAnalysis\n");
        printf("21. Exit\n");

        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;


            case 3:
                addTrain();
                break;

            case 4:
                viewTrains();
                break;

            case 5:
                bookTicket();
                break;

            case 6:
                cancelTicket();
                
                break;

            case 7:
                viewReservation();
                                break;

            case 8:
                makePayment();
                break;

            case 9:
                generateReport();
                break;

            case 10:
                runStressTest();
                break;

            
            case 11:
                forgotPassword();
                break;

            case 12:
                updateTrainSchedule();
                break;
            case 13:
                viewWaitlist();
                break;
            case 14:
                generateRevenueReport();
                break;
            case 15:
                viewLoginHistory();
                break;

            case 16:
                viewNotifications();
                break;
            case 17:
                partialCancellation();
                break;
            case 18:
                {
                    int freeMemory;

                    freeMemory = getAvailableMemory();

                    printf("\n===== OOM ANALYSIS =====\n");
                    printf("Available Memory : %d KB\n",
                        freeMemory);

                    if(freeMemory < 50000)
                    {
                        printf("WARNING : MEMORY PRESSURE DETECTED\n");

                        logOOMEvent();
                        addUserToQueue("BOOKING_REQUEST");
                    }
                    else
                    {
                        printf("Memory Status : HEALTHY\n");
                    }

                    break;
                }
            case 19:
                showOOMDashboard();
                break;
            case 20:
                runOOMAnalysis();
                break;
            case 21:
                printf("Exiting Application...\n");
                exit(0);

            default:
                printf("Invalid Choice! Please Try Again.\n");
        }
    }
   
    return 0;
}
