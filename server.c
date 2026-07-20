#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX_BOOKINGS 100
typedef struct{
    int bookingId;
    char passengerName[50];
    int age;
    int seatNumber;
    char status[20];
} Booking;

Booking bookings[MAX_BOOKINGS];
int bookingCount = 0;

void saveBookings(){
    FILE *fp = fopen("bookings.dat", "wb");
    if(fp == NULL)
        return;

    fwrite(&bookingCount, sizeof(int), 1, fp);
    fwrite(bookings, sizeof(Booking), bookingCount, fp);

    fclose(fp);
}

void loadBookings(){
    FILE *fp = fopen("bookings.dat", "rb");

    if(fp == NULL)
        return;

    fread(&bookingCount, sizeof(int), 1, fp);
    fread(bookings, sizeof(Booking), bookingCount, fp);

    fclose(fp);
}

void addBooking(char *name, int age)
{
    if(bookingCount >= MAX_BOOKINGS)
        return;

    Booking b;

    b.bookingId = bookingCount + 1;
    strcpy(b.passengerName, name);
    b.age = age;
    b.seatNumber = bookingCount + 1;
    strcpy(b.status, "CONFIRMED");

    bookings[bookingCount++] = b;

    saveBookings();
}

int main()
{
    int server_fd;
    int client_socket;

    struct sockaddr_in server_addr;

    char buffer[1024];

    loadBookings();

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr));

    listen(server_fd, 5);

    printf("Server Started...\n");

    while(1)
    {
        client_socket =
            accept(server_fd, NULL, NULL);

        memset(buffer, 0, sizeof(buffer));

        read(client_socket,
             buffer,
             sizeof(buffer));

        printf("Request: %s\n", buffer);

        if(strncmp(buffer, "BOOK", 4) == 0)
        {
            char name[50];
            int age;

            sscanf(buffer,
                   "BOOK %s %d",
                   name,
                   &age);

            addBooking(name, age);

            write(client_socket,
                  "BOOKING SUCCESSFUL",
                  18);
        }
        else if(strncmp(buffer, "VIEW", 4) == 0)
        {
            char response[4096];

            strcpy(response,
                   "----- BOOKINGS -----\n");

            for(int i = 0;
                i < bookingCount;
                i++)
            {
                char temp[200];

                sprintf(temp,
                        "ID:%d Name:%s Age:%d Seat:%d Status:%s\n",
                        bookings[i].bookingId,
                        bookings[i].passengerName,
                        bookings[i].age,
                        bookings[i].seatNumber,
                        bookings[i].status);

                strcat(response, temp);
            }

            write(client_socket,
                  response,
                  strlen(response));
        }
        else
        {
            write(client_socket,
                  "INVALID COMMAND",
                  15);
        }

        close(client_socket);
    }

    close(server_fd);

    return 0;
}
