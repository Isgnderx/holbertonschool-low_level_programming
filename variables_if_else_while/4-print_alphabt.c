#include <stdio.h>

/**
 *  main - Prints the lowercase alphabetexcept q and e following the new line
 * 
 * 
 *  Return: Always 0
 */

 int main(void)
 {
    char c;

    for (c = 'a'; c <= 'z'; c++)
    {
        if (c != 'q' && c != 'e')
        {
putchar(c);
        }
    }
putchar('\n');
return (0);
 }