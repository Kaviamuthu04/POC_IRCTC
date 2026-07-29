#include <stdio.h>
#include <assert.h>

int main()
{
    int availableSeats = 10;

    availableSeats--;

    assert(availableSeats == 9);

    printf("Booking Test Passed\n");

    return 0;
}