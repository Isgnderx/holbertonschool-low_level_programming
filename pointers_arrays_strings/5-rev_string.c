#include "main.h"

/**
 * rev_string - Reverses a string in place
 * @s: Pointer to the string to reverse
 *
 * Return: void
 */
void rev_string(char *s)
{
	int i = 0;
	int j = 0;
	char temp;

	while (s[j] != '\0')
		j++;

	j--; /* Set j to last character index */

	while (i < j)
	{
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		i++;
		j--;
	}
}
