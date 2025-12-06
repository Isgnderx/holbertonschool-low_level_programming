#include "main.h"

/**
 * print_alphabet_x10 - prints the alphabet 10 times in lowercase
 *
 * Description: Uses a loop to print letters from 'a' to 'z'
 * ten times, each followed by a new line.
 */
void print_alphabet_x10(void)
{
	int i;
	char c;

	for (i = 0; i < 10; i++)
	{
		for (c = 'a'; c <= 'z'; c++)
		{
			_putchar(c); /* 1. kullanım */
		}
		_putchar('\n');   /* 2. kullanım */
	}
}

