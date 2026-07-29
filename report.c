#include <stdio.h>
#include <string.h>
void generateReport()
{
    FILE *fp;

    fp = fopen(
    "reports/daily_report.txt",
    "w");

    fprintf(fp,
    "IRCTC Daily Booking Report\n");

    fprintf(fp,
    "Report Generated Successfully\n");

    fclose(fp);

    printf("Report Created\n");
}
#include <stdio.h>
#include <string.h>
#include <time.h>

void generateRevenueReport()
{
    FILE *fp;
    FILE *reportFp;

    char line[200];

    float amount;
    float totalRevenue = 0;

    fp = fopen("data/payments.dat","r");

    if(fp == NULL)
    {
        printf("Payment File Not Found\n");
        return;
    }

    while(fgets(line,sizeof(line),fp))
    {
        if(strncmp(line,"Amount:",7) == 0)
        {
            sscanf(line,
                   "Amount:%f",
                   &amount);

            totalRevenue += amount;
        }
    }

    fclose(fp);

    printf("\n==============================\n");
    printf("       REVENUE REPORT\n");
    printf("==============================\n");
    printf("Total Revenue : %.2f\n",
           totalRevenue);
    printf("==============================\n");

    reportFp = fopen("reports/revenue_report.txt","a");

    if(reportFp == NULL)
    {
        printf("Report File Error\n");
        return;
    }

    time_t t;
    t = time(NULL);

    fprintf(reportFp,
            "\n========================================\n");

    fprintf(reportFp,
            "      IRCTC REVENUE REPORT\n");

    fprintf(reportFp,
            "========================================\n");

    fprintf(reportFp,
            "Generated On : %s",
            ctime(&t));

    fprintf(reportFp,
            "Total Revenue : %.2f\n",
            totalRevenue);

    fprintf(reportFp,
            "========================================\n");

    fclose(reportFp);

    printf("Revenue Report Saved Successfully\n");
}
