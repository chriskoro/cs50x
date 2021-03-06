/* Prints user initials with user providing name 
Usage: initials */

#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    string name;
    name = GetString();
    
    for (int i = 0, n = strlen(name); i < n; i++)
    {
        if (i == 0)
        {
            printf("%c", toupper(name[i]));
        }  
        if (name[i] == ' ')
        {
            printf("%c", toupper(name[i+1]));
        }
    }
    
    printf("\n");
}