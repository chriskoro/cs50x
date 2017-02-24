/*
User inputs shower length and program
prints water bottles used

Usage: water
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