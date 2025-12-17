#include "main.h"

int _prime_helper(int n, int i);

/**
 * is_prime_number - returns 1 if the number is prime, otherwise 0
 * @n: number to check
 *
 * Return: 1 if n is prime, otherwise 0
 */
int is_prime_number(int n)
{
	if (n <= 1)
		return (0);

	return (_prime_helper(n, 2));
}

/**
 * _prime_helper - checks if a number is prime recursively
 * @n: number to check
 * @i: possible divisor
 *
 * Return: 1 if prime, otherwise 0
 */
int _prime_helper(int n, int i)
{
	if (i * i > n)
		return (1);

	if (n % i == 0)
		return (0);

	return (_prime_helper(n, i + 1));
}

