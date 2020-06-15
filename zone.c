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

inline size_t	align_block(size_t x)
{
	if (x < 2)
		x = 2;
	return (((x - 1) / sizeof(t_block) + 1) * sizeof(t_block));
}

inline size_t	align_page(size_t x)
{
	if (x < 2)
		x = 2;
	return (((x - 1) / getpagesize() + 1) * getpagesize());
}

void			set_zone(t_zone *zone, size_t size, size_t type)
{
	zone->type = type;
	zone->size = align_page(size);
	zone->available = zone->size - sizeof(t_block);
	zone->begin = mmap(NULL, zone->size,
						PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	zone->end = zone->begin + zone->size;
	zone->head = (t_block*)zone->begin;
	set_block_info(zone->head, NULL, NULL, zone->available);
	zone->head->is_free = 1;
	zone->tail = zone->head;
	zone->first_free = zone->head;
}

t_zone			*get_zone(size_t size)
{
	static int		is_set = 0;
	static t_zone	zones[2];

	if (!is_set)
	{
		set_zone(&zones[S], PAGES_S * getpagesize(), S);
		set_zone(&zones[M], PAGES_M * getpagesize(), M);
		is_set = 1;
	}
	if (size <= SIZE_S)
	{
		return (&zones[S]);
	}
	else if (size <= SIZE_M)
	{
		return (&zones[M]);
	}
	return (NULL);
}

void			update_zone(t_zone *zone, t_block *tmp, size_t size)
{
	if (zone->tail < tmp)
		zone->tail = tmp;
	if (tmp->is_free && (zone->first_free > tmp || !zone->first_free))
		zone->first_free = tmp;
	if (!tmp->is_free && zone->first_free == tmp)
		zone->first_free = 0;
	zone->available -= size;
}
