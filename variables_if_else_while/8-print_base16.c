#include <stdio.h>

/**
 * main - Prints all the numbers of base 16 in 
 * lowercase, followed by a new line.
 *
 * Return: Always 0
 */
int main(void)
{
	char c;

	int a;

	for (a = 0; a <= 9; a++)
	{
		putchar(a + '0');
	}

	for (c = 'a'; c <= 'f'; c++)
	{
		putchar(c);
	}

	putchar('\n');

	return (0);
}
