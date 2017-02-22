#include <stdio.h>
#include <stdlib.h>

void say_hello(char*name)
{
    printf("Hello, %s\n", name);
}

int main(void)
{
    
    say_hello("Chris");
    
    return 0;
}