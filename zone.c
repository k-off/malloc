/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zone.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void				defragment(t_zone *zone, t_block *block)
{
	while (block->next && block->next->is_free)
		block = block->next;
	while (block->prev && block->prev->is_free)
	{
		block->prev = set_block_info(block->prev, block->prev->prev,
				block->next, block->prev->size + sizeof(t_block) + block->size);
		update_zone(zone, block, REMOVE);
		block->magic = 0;
		block->size = 0;
		block = block->prev;
	}
	if (block->is_free && block < zone->first_free)
		zone->first_free = block;
	while (zone->tail->prev && !zone->tail->size)
		zone->tail = zone->tail->prev;
}

void				update_zone(t_zone *zone, t_block *block, int action)
{
	zone->available -= (sizeof(t_block) * action * block->is_free);
	zone->available -= (block->size * !block->is_free * action);
	if (block > zone->tail)
		zone->tail = block;
	while (!zone->first_free->is_free && zone->first_free->next)
		zone->first_free = zone->first_free->next;
}

/*
** initial zone setup
*/

static t_zone		*set_zone(t_zone **zone, size_t page_count)
{
	if (!*zone)
	{
		*zone = mmap(NULL, (page_count * getpagesize()),
					PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		(*zone)->size = (page_count * getpagesize());
		(*zone)->available = (*zone)->size - sizeof(t_zone) - sizeof(t_block);
		(*zone)->head = (t_block*)((*zone) + 1);
		(*zone)->head =
			set_block_info((*zone)->head, NULL, NULL, (*zone)->available);
		(*zone)->head->is_free = 1;
		(*zone)->tail = (*zone)->head;
		(*zone)->first_free = (*zone)->head;
	}
	return (*zone);
}

t_zone				*get_zone(size_t size)
{
	static int		is_set = 0;

	if (!is_set)
	{
		is_set = 1;
		set_zone(&g_zone[0], SMALL_ZONE_PAGE_COUNT);
		set_zone(&g_zone[1], MEDIUM_ZONE_PAGE_COUNT);
		set_zone(&g_zone[2], 1);
	}
	if (size <= SMALL_BLOCK && g_zone[0]->available > size)
		return (g_zone[0]);
	else if (size <= MEDIUM_BLOCK && g_zone[1]->available > size)
		return (g_zone[1]);
	else
		return (g_zone[2]);
}
