#include "main.h"

/**
 * puts_half - prints the second half of a string
 * @str: the string to process
 *
 * Return: void
 */
void puts_half(char *str)
{
	int len = 0;
	int start;

	while (str[len] != '\0')
		len++;

	if (len % 2 == 0)
		start = len / 2;
	else
		start = (len + 1) / 2;

	while (start < len)
	{
		_putchar(str[start]);
		start++;
	}

	_putchar('\n');
}

