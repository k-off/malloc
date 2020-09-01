#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MIB (1024 * 1024)

void print(char *s)
{
	write(1, s, strlen(s));
}

int		main(void)
{
	char *addr1;
	char *addr3;

	addr1 = (char*)malloc(16*MIB);
	strcpy(addr1, "Bonjours\n");
	print(addr1);
	addr3 = (char*)realloc(addr1, 128*MIB);
	addr3[127*MIB] = 42;
	print(addr3);
	return (0);
}
