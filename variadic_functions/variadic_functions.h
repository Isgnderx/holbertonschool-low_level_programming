#ifndef VARIADIC_FUNCTIONS_H
#define VARIADIC_FUNCTIONS_H

#include <stdarg.h>

/* Struct for mapping format char to function */
typedef struct printer
{
	char c;
	void (*f)(va_list);
} printer_t;

/* Function prototypes */
void print_all(const char * const format, ...);
void print_char(va_list args);
void print_int(va_list args);
void print_float(va_list args);
void print_string(va_list args);

#endif /* VARIADIC_FUNCTIONS_H */

