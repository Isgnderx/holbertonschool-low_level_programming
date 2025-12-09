#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: input string
 *
 * Return: the integer converted from the string
 */
int _atoi(char *s)
{
	int i = 0, sign = 1, started = 0, result = 0, digit;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
			sign = -sign;
		else if (s[i] >= '0' && s[i] <= '9')
		{
			started = 1;
			digit = s[i] - '0';

			/* Build as negative to safely handle INT_MIN */
			result = result * 10 - digit;
		}
		else if (started)
			break;

		i++;
	}

	if (sign == 1)
		result = -result;

	return (result);
}

