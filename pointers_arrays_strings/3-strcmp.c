#include "main.h"

/**
 * _strcmp - compares two strings, works exactly like strcmp
 * @s1: first string
 * @s2: second string
 *
 * Return: negative, zero, or positive integer depending on comparison
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}

	return (s1[i] - s2[i]);
}

