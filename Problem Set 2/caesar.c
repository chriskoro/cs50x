/* This program uses the Caesar Cipher to encrypt the user's given message
by the value that the user provides (ie if user provides 3 and plaintext "hi", 
program will show "kl") */

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Please enter one whole number as key\n");
        return 1;
    }
    
    //convert k user provided to int
    int k = atoi(argv[1]);
    k = k % 26;

    //prompt user for plaintext
    string plaintext = GetString();
    
    //display ciphertext
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        int c;
        
        if (isupper(plaintext[i]))
        {
            c = ((((int)plaintext[i] + k -65) % 26) + 65);
            printf("%c", c);
        }
        else if (islower(plaintext[i]))
        {
            c = ((((int)plaintext[i] + k - 97) % 26) + 97);
            printf("%c", c);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
        
    }
    
    printf("\n");
    
    return 0;
    
}
