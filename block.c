/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   block.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void			munmap_block(t_block *block)
{
	if (block->prev)
		block->prev->next = block->next;
	if (block->next)
		block->next->prev = block->prev;
	if (g_zone[2]->tail == block)
		g_zone[2]->tail = block->prev;
	block->magic = 0;
	munmap(block, block->size + sizeof(t_block));
}

t_block			*set_block_info(t_block *block, t_block *previous,
								t_block *next, size_t size)
{
	block->prev = previous;
	block->next = next;
	block->size = size;
	block->magic = MAGIC;
	if (previous)
	{
		previous->next = block;
	}
	if (next)
	{
		next->prev = block;
	}
	return (block);
}

/*
** mmap a block if not enough space in preallocated areas or block is too big;
** link newly created block to the end of big heap list
*/

static t_block	*get_big_block(t_zone *zone, size_t size)
{
	t_block		*temporary_block;

	size = align_size(size, getpagesize());
	temporary_block = (t_block*)mmap(NULL, size,
						PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	temporary_block->is_free = 0;
	set_block_info(zone->tail, zone->tail->prev, temporary_block,
				zone->tail->size);
	set_block_info(temporary_block, zone->tail, NULL, size - sizeof(t_block));
	zone->tail = temporary_block;
	return (temporary_block);
}

/*
** in preallocated areas, mark a block as used; if there is enough space
** in between current block and next one - insert an empty block
*/

static t_block	*insert_block(t_zone *zone, t_block *block, size_t size)
{
	t_block		*temp_block;

	block->is_free = 0;
	if (block->size > size + sizeof(t_block) +
		(size > SMALL_BLOCK ? align_size(SMALL_BLOCK + 1, sizeof(size_t)) :
		sizeof(size_t)))
	{
		temp_block = set_block_info(((void*)block) + sizeof(t_block) + size,
					block, block->next,
					block->size - (size + sizeof(t_block)));
		temp_block->is_free = 1;
		update_zone(zone, temp_block, ADD);
	}
	block->size = size;
	update_zone(zone, block, ADD);
	return (block);
}

t_block			*get_block(t_zone *zone, size_t size)
{
	t_block		*block;

	block = NULL;
	if (zone == g_zone[2])
		return (get_big_block(zone, size));
	block = zone->first_free;
	while (block->next && (block->size < size || !block->is_free))
		block = block->next;
	if (!block->is_free)
		return (NULL);
	block = insert_block(zone, block, size);
	return (block);
}
