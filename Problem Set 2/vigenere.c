/* Program uses the Vigenere cipher to encrypt a plaintext 
message given by the user with the keyword given by the user */

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    
    //make sure user only entered one command-line argument
    if (argc != 2)
    {
        printf("Please enter only one argument, an alphabetic key.\n");
        return 1;
    }
    
    string key = argv[1];
    
    
    //make sure command-line keyword is valid
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("please give an alphabetic key.");
            return 1;
        }
    }
        
    //get string from user
    string plaintext = GetString();
    
    int c;
    //iterate through plaintext and cipher each character in it by the key (ignore spaces)
    for (int i = 0, j = 0, n = strlen(plaintext); i < n; i++, j++)
    {
        if (isupper(plaintext[i]))
        {
            if (j == strlen(key))
                j = 0;
            
            c = (((((int)plaintext[i] + (int)toupper(key[j])) - 65 - 65) % 26) + 65);
            printf("%c", c);
        }
        else if(islower(plaintext[i]))
        {
            if (j == strlen(key))
                j = 0;
                
            c = (((((int)plaintext[i] + (int)tolower(key[j])) - 97 - 97) % 26) + 97);
            printf("%c", c);
        }
        else if (plaintext[i] == ' ')
        {
            printf("%c", plaintext[i]);
            j -=1;
        }
        /* TEST CASE  keyword: bacon  plaintext: Meet me at the park at eleven am 
        cipertext: Negh zf av huf pcfx bt gzrwep oz */
    }

    printf("\n");
    return 0;
    
}