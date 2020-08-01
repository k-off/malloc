/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		free_zone(t_zone *zone, t_block *block)
{
	block->is_free = 1;
	zone->available += block->size;
	while (block->prev && block->prev->is_free)
	{
		block = block->prev;
		if (zone->first_free >= block)
			zone->first_free = block;
	}
	while (block->next && block->next->is_free)
	{
		zone->available += sizeof(t_block);
		block->size += block->next->size + sizeof(t_block);
		block->next->magic = 0;
		block->next = block->next->next;
	}
}

t_block		*manage_big_list(t_block *add, t_block *rem)
{
	static t_block	*list = NULL;
	t_block			*tmp;

	if (add && (!list || list > add))
	{
		set_block_info(add, list, NULL, add->size);
		list = add;
	}
	else if (add && list && list < add)
	{
		tmp = list;
		while (tmp < add && tmp->next && tmp->next < add)
			tmp = tmp->next;
		add->prev = tmp;
		if (tmp->next)
			tmp->next->prev = add;
		add->next = tmp->next;
		tmp->next = add;
	}
	if (list && rem && rem == list)
	{
		list = list->next;
	}
	return (list);
}

t_block		*save_big_blocks(size_t size)
{
	t_block *block;

	size = align_page(size + sizeof(t_block));
	block = mmap(NULL, size,
				PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	set_block_info(block, NULL, NULL, size - sizeof(t_block));
	manage_big_list(block, NULL);
	return (block);
}

void		free_big_blocks(t_block *block)
{
	t_block		*tmp;

	tmp = block;
	if (tmp->prev)
	{
		tmp->prev->next = tmp->next;
	}
	if (tmp->next)
	{
		tmp->next->prev = tmp->prev;
	}
	tmp->magic = 0;
	manage_big_list(NULL, tmp);
	munmap(tmp, align_page(tmp->size));
}

void		free(void *ptr)
{
	t_block	*block;
	t_zone	*zone;

	if (!ptr || ptr < get_zone(SIZE_S)->begin)
		return ;
	block = ((t_block*)ptr) - 1;
	if (block->magic != MAGIC)
		return ;
	zone = get_zone(SIZE_S);
	if (zone->begin <= (void*)block && zone->end > (void*)block)
	{
		free_zone(zone, block);
		return ;
	}
	zone = get_zone(SIZE_M);
	if (zone->begin <= (void*)block && zone->end > (void*)block)
	{
		free_zone(zone, block);
		return ;
	}
	free_big_blocks(block);
}
