/*
Gives the amount of bottles used for the length of shower given 
by user (not from command prompt yet, but from GetString() from 
cs50 library).
*/
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Please input the length of your shower in minutes: ");
    int minutes = GetInt();
    int bottles = (minutes * 12);
    printf("You use %i 16-ounce bottles of water when taking a shower!\n", bottles);
}
