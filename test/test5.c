#include "../malloc.h"
#include <string.h>

void print(char *s)
{
	write(1, s, strlen(s));
}

int main()
{
	void *pointer;
	pointer = malloc(1024);
	pointer = malloc(1024 * 32);
	pointer = malloc(1024 * 1024);
	pointer = malloc(1024 * 1024 * 16);
	pointer = malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return(0);
}
