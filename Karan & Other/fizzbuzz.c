   /*print numbers 1 to 100,
   for multiples of 3 print fizz, mults of 5 buzz
   multiples of both 3 and 5 fizz buzz
   */
   
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    for (int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0 && i % 5 != 0)
        {
            printf("fizz\n");
        }
        else if (i % 5 == 0 && i % 3 != 0)
        {
            printf("buzz\n");
        }
        else if (i % 5 == 0 && i % 3 == 0)
        {
            printf("fizz-buzz\n");
        }
        else
        {
            printf("%i\n", i);
        }
    }
   
   
   //took me 9 minutes! (1/8/2016, 5:35am)
}

