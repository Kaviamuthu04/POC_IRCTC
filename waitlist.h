#ifndef WAITLIST_H
#define WAITLIST_H
void promoteRAC();
void promoteWaitlist();
void promoteWaitlistPassenger(int cancelledTrainNo);
void addToWaitlist(char name[]);
void viewWaitlist();
#endif