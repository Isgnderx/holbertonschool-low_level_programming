#ifndef FUNCTION_POINTERS_H
#define FUNCTION_POINTERS_H

#include <stddef.h>  /* size_t, NULL */

void print_name(char *name, void (*f)(char *));

/* Example prototype using size_t */
void array_iterator(int *array, size_t size, void (*action)(int));

#endif /* FUNCTION_POINTERS_H */
