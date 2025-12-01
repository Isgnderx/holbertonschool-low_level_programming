#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - Prints the last digit of a random number
 *
 * Description: Generates a random number, stores it in n,
 *              and prints its last digit along with a message
 *              depending on whether it is greater than 5,
 *              equal to 0, or less than 6 and not 0.
 *
 * Return: Always 0.
 */
int main(void)
{
	int n, last_digit;

	srand(time(0));
	n = rand() - RAND_MAX / 2;

	last_digit = n % 10;
	if (last_digit < 0)
	{
		last_digit = -last_digit;
	}

	if (last_digit > 5)
	{
		printf("Last digit of %d is %d and is greater than 5\n", n, last_digit);
	}
	else if (last_digit == 0)
	{
		printf("Last digit of %d is %d and is 0\n", n, last_digit);
	}
	else
	{
		printf("Last digit of %d is %d and is less than 6 and not 0\n", n, last_digit);
	}

	return (0);
}

