#include "main.h"

/**
 * print_alphabet - prints the lowercase alphabet followed by a new line
 *
 * Description: This function prints letters from 'a' to 'z'
 * using only two occurrences of the _putchar function in the code.
 */
void print_alphabet(void)
{
	char c;

	for (c = 'a'; c <= 'z'; c++)
	{
		_putchar(c);
	}
	_putchar('\n');
}

