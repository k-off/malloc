#include "../malloc.h"
#include <string.h>
#include <stdio.h>

void	pause_execution(void)
{
	char key;
	printf("Press any key to continue...\n");
	scanf("%c",&key);
}

void	test_defragment(void)
{
	void	*ptrs[5];
	
	printf("\nAllocate pointers:\n");
	for (int i = 0; i < 5; i++) {
		ptrs[i] = malloc(1);
	}
	show_alloc_mem_ex(STANDARD);
	printf("\nFree pointers #1 and #3:\n");
	free(ptrs[1]);
	free(ptrs[3]);
	show_alloc_mem_ex(STANDARD);
	printf("\nFree pointer #2, merge it with #1 and #3:\n");
	free(ptrs[2]);
	show_alloc_mem_ex(STANDARD);
	free(ptrs[0]);
	free(ptrs[4]);
}

void	test_print_memory(void)
{
	void	*addr;

	addr = malloc(33);
	ft_strcpy(addr, "Tiny allocation\n\0");
	printf("pointer: %p\n", addr);
	show_alloc_mem_ex(EXTENDED);
	pause_execution();
	free(addr);
	addr = malloc(513);
	ft_strcpy(addr, "Small allocation\n\0");
	show_alloc_mem_ex(EXTENDED);
	free(addr);
}

int		main(void)
{
	printf("\nTest memory defragmentation:\n");
	pause_execution();
	test_defragment();
	printf("\nTest memory printing:\n");
	pause_execution();
	test_print_memory();
	return (0);
}
