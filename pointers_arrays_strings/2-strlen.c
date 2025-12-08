#include "main.h"
#include <stdio.h>

/**
 * main - check the code
 *
 * Return: Always 0.
 */

int _strlen(char *s)
{

    int len = 0;
    
    while(*(s) != '\0')
    {
        len++;
        s++;
    }

    return len;
}

