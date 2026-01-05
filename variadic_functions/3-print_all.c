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
	unsigned int i = 0, j;
	char *sep = "";
	char *s;

	typedef struct printer
	{
		char c;
		void (*f)(va_list);
	} printer_t;

	void print_char(va_list a) { printf("%c", va_arg(a, int)); }
	void print_int(va_list a) { printf("%d", va_arg(a, int)); }
	void print_float(va_list a) { printf("%f", va_arg(a, double)); }
	void print_string(va_list a)
	{
		s = va_arg(a, char *);
		printf("%s", s ? s : "(nil)");
	}

	printer_t p[] = {
		{'c', print_char},
		{'i', print_int},
		{'f', print_float},
		{'s', print_string},
		{0, NULL}
	};

	va_start(args, format);

	while (format && format[i])
	{
		j = 0;
		while (p[j].c)
		{
			if (format[i] == p[j].c)
			{
				printf("%s", sep);
				p[j].f(args);
				sep = ", ";
				break;
			}
			j++;
		}
		i++;
	}

	va_end(args);
	printf("\n");
}

