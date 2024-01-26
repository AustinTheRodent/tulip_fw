
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
    char str[100];

    printf( "Enter a value :");
    gets(str);

    printf( "\nYou entered: ");
    printf("%s\n", str);
    while(1)
    {
        gets(str);
        if(strcmp(str, "") != 0)
        {
            break;
        }
    }

    return 0;
}
