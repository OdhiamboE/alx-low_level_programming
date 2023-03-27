#include "main.h"

/**
 * swap_int - swap variable values
 * @a: first int to swap
 * @b: second int to swap
 * Return: void
 */

void swap_int(int *a, int *b)
{
	int i;

	i = *a;
	*a = *b;
	*b = i;
}
