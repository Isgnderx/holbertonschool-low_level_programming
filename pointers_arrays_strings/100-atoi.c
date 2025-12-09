#include "main.h"

/**
 * _atoi - convert a string to an integer
 * @s: string to convert
 *
 * Return: the integer value
 */
int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int result = 0;
	int started = 0;
	int digit;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
			sign = -sign;
		else if (s[i] >= '0' && s[i] <= '9')
		{
			started = 1;

			digit = s[i] - '0';

			/* BUILD AS NEGATIVE TO AVOID OVERFLOW */
			if (sign == 1)
			{
				if (result < -214748364)  /* would overflow if made positive later */
					return (2147483647);   /* INT_MAX */
				if (result == -214748364 && digit > 7)
					return (2147483647);   /* INT_MAX */
			}
			else
			{
				if (result < -214748364)
					return (-2147483648);  /* INT_MIN */
				if (result == -214748364 && digit > 8)
					return (-2147483648);  /* INT_MIN */
			}

			result = result * 10 - digit;
		}
		else if (started)
			break;

		i++;
	}

	/* Convert to positive only if needed */
	if (sign == 1)
		result = -result;

	return (result);
}

