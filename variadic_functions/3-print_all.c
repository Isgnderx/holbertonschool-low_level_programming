#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_all - prints anything
 * @format: list of argument types
 *
 * Return: void
 */
void print_all(const char * const format, ...)
{
	va_list args;
	unsigned int i = 0;
	char *str;
	char *sep = "";
	char c;
	int num;
	double f;

	va_start(args, format);

	while (format && format[i])
	{
		while (format[i] != 'c' && format[i] != 'i' &&
		       format[i] != 'f' && format[i] != 's')
			i++;

		printf("%s", sep);
		sep = ", ";

		if (format[i] == 'c')
			c = va_arg(args, int), printf("%c", c);
		if (format[i] == 'i')
			num = va_arg(args, int), printf("%d", num);
		if (format[i] == 'f')
			f = va_arg(args, double), printf("%f", f);
		if (format[i] == 's')
			str = va_arg(args, char *),
			printf("%s", str ? str : "(nil)");

		i++;
	}

	va_end(args);

	printf("\n");
}

