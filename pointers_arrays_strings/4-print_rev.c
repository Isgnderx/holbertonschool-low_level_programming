#include "main.h"

/**
 * print_rev - writes the string in the reverse
 * @s: I do not fear computers.
 * I fear the lack of them - Isaac Asimov
 *
 * Return: void
 */
void print_rev(char *s)
{
	int len = 0;


	while (s[len] != '\0')
		len++;

	for (len = len -1; len >= 0; len--)
		_putchar(s[len]);

	_putchar('\n')
}
