/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   output.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	print_meta(t_block *tmp, int mode)
{
	size_t	block_begin;
	size_t	block_end;
	size_t	data_begin;
	size_t	block_size;
	size_t	data_size;

	if (!mode)
		return ;
	block_begin = (size_t)tmp;
	block_end = (size_t)(tmp + 1 + tmp->size / sizeof(t_block));
	data_begin = (size_t)(tmp + 1);
	block_size = data_begin - block_begin;
	data_size = block_end - data_begin;
	write(1, "\n\tMetadata\n\tBlock address: ", 27);
	print_pointer(block_begin);
	write(1, "\n\tData address : ", 17);
	print_pointer(data_begin);
	write(1, "\n\tEnd of block : ", 17);
	print_pointer(block_end);
	write(1, "\n\tBlock size   : ", 17);
	print_size_t(block_size, 1);
	write(1, "\n\tData size    : ", 17);
	print_size_t(data_size, 1);
	write(1, "\n", 1);
}

void	print_block(t_block *block, int mode)
{
	size_t begin;
	size_t end;

	print_meta(block, mode);
	if (mode)
		write(1, block->is_free ? "\tBlock is free\n" :
			"\tBlock is used\n", 15);
	if (!mode && block->is_free)
		return ;
	begin = (size_t)(block + 1);
	end = (size_t)(block + 1 + block->size / sizeof(t_block));
	print_pointer(begin);
	write(1, " - ", 3);
	print_pointer(end);
	write(1, " : ", 3);
	print_size_t(end - begin, 1);
	write(1, " bytes\n", 7);
	if (!block->is_free && mode > STD)
	{
		write(1, "\tData:\n", 7);
		print_memory("\t\t", block + 1, block->size);
	}
}

size_t	print_zone(t_zone *zone, int mode, size_t total_allocated)
{
	t_block		*tmp;

	if (!zone)
	{
		return (0);
	}
	write(1, zone->type == M ? "SMALL : " : "TINY  : ", 8);
	print_pointer((size_t)zone->begin);
	write(1, "\n", 1);
	tmp = zone->head;
	while (tmp)
	{
		total_allocated += tmp->size * !tmp->is_free;
		print_block(tmp, mode);
		tmp = tmp->next;
	}
	return (total_allocated);
}

void	show_alloc_mem_ex(int mode)
{
	size_t		total;
	t_block		*big;

	total = print_zone(get_zone(SIZE_S), mode, 0);
	total += print_zone(get_zone(SIZE_M), mode, 0);
	big = manage_big_list(NULL, NULL);
	if (big)
	{
		write(1, "LARGE : ", 8);
		print_pointer((size_t)big);
		write(1, "\n", 1);
	}
	while (big)
	{
		print_block(big, mode);
		total += big->size;
		big = big->next;
	}
	write(1, "Total : ", 8);
	print_size_t(total, 1);
	write(1, "\n", 1);
}

void	show_alloc_mem(void)
{
	show_alloc_mem_ex(SHRT);
}
