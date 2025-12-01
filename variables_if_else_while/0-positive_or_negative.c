#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - Entry point
 *
 * Description: Generates a random number and prints whether
 *              it is positive, zero, or negative.
 *
 * Return: Always 0.
 */
int main(void)
{
	int n;

	srand(time(NULL));
	n = rand() - RAND_MAX / 2;  /* n can be negative and positive */

	if (n > 0)
	{
		printf("%d is positive\n", n);
	}
	else if (n == 0)
	{
		printf("%d is zero\n", n);
	}
	else
	{
		printf("%d is negative\n", n);
	}

	return (0);
}
