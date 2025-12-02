#include <stdio.h>

/**
 * main - prints all different combinations of two digits
 * 
 * Return: Always 0
 */
int main(void)
{
	int i, j;

	for (i = 0; i <= 8; i++)
	{
        for (j = i + 1; j <= 9; j++)
        {
		    putchar(i + '0');
            putchar(j + '0');

            if (!(i == 0 && j ==9))
		{
			putchar(',');
			putchar(' ');
		}
        }
	}

	putchar('\n');

	return (0);
}
