#ifndef LOGGER_H
#define LOGGER_H
void logCancellation(int pnr);
void writeLog(char message[]);
void logBooking(int pnr);
void logPayment(int pnr);
void logOOMEvent(void);
void logLogin(char username[]);
#endif