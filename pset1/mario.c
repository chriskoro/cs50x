/*
User enters height of mario pyramid during
runtime and program prints it of that size.

Usage: mario
*/

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask user
    
    int height;
    do
    {
        printf("Height: ");
        height = GetInt();
        
        if (height == 0)
            return 0;
            
        if (height > 23)
            printf("Height must be between 1 and 23\n");
            
    }
    while(height < 1 || height > 23);
    
    
    //print pyramid
    
    int row;
    
    for (row = 1; row <= height; row++)
    {
        for (int i = 0; i <= (height-row); i++)
        {
            printf(" ");
        }
        for (int i = 0; i <= row; i++)
        {
            printf("#");
        }
    
        printf("\n");
    }
    
}

