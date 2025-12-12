#include "main.h"

/**
 * _strchr - locates a character in a string
 * @s: the string to search in
 * @c: the character to find
 *
 * Return: pointer to the first occurrence of c in s,
 * or NULL if the character is not found
 */
char *_strchr(char *s, char c)
{
	int i;

	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == c)
			return (s + i);
	}

	/* Check for the null terminator case if c == '\0' */
	if (c == '\0')
		return (s + i);

	return (NULL);
}

