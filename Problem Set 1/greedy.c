/* This program implements the Greedy algorithm to find the least
amount of coins to give back to the user (after the user provides 
the change amount, in dollars). For instance, .41 or 41 cents is
4 coins (a quarter, a dime, a nickel, a penny)*/

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void)
{
    
    int loopCount = 0;
    float dollars;
    
    do
    {
        loopCount++;
        
        if(loopCount < 2)
            printf("O hai! How much change is owed? \n");
        
        dollars = GetFloat();
        
        if (dollars <= 0)
            printf("Number should be greater than zero (i.e. for 10 cents, put .10)\n");
    }
    while(dollars<=0);
    

    int change = round(dollars * 100);
  
    int coins = 0;
    
    while (change >= 25)
    {
        change -= 25;
        coins++;
    }
    while (change >= 10)
    {
        change -= 10;
        coins++;
    }
    while (change >= 5)
    {
        change -= 5;
        coins++;
    }
    while (change >= 1)
    {
        change -= 1;
        coins++;
    }
    
    printf("%i\n", coins);
    
}
