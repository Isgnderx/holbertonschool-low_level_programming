#include <stdio.h>

/**
 * main - prints all different combinations of two digits
 *
 * Return: Always 0
 */
int main(void)
{
	int i, j, k;

	for(k = 0; k <= 7; k++)
    {
        for (i = 0; i <= 8; i++)
	    {
		    for (j = i + 1; j <= 9; j++)
		    {
			    putchar(i + '0');
			    putchar(j + '0');
                putchar(k + '0');

			    if (!(k == 7&&i == 8 && j == 9))
			    {
				    putchar(',');
				    putchar(' ');
			    }
		    }
	    }
    }
	putchar('\n');

	return (0);
}
