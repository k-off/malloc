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

void	set_block_info(t_block *block, t_block *p, t_block *n, size_t size)
{
	block->prev = p;
	block->next = n;
	block->size = size;
	block->magic = MAGIC;
	if (p)
	{
		p->next = block;
	}
	if (n)
	{
		n->prev = block;
	}
}

t_block	*append_block(t_zone *zone, t_block *new_bl, size_t size)
{
	t_block		*tmp;
	void		*end;

	end = new_bl->next ? new_bl->next : zone->end;
	tmp = new_bl + new_bl->size + sizeof(t_block);
	if (end - (void*)tmp - size - sizeof(t_block) < 0)
	{
		return (NULL);
	}
	new_bl->next = tmp;
	set_block_info(tmp, new_bl, NULL, size);
	tmp->is_free = 0;
	update_zone(zone, tmp, (size + sizeof(t_block)));
	return (tmp);
}

void	insert_free_block(t_block *new_bl, t_zone *zone)
{
	t_block		*tmp;
	void		*end;

	end = new_bl->next ? new_bl->next : zone->end;
	tmp = new_bl + (new_bl->size / sizeof(t_block) + 1);
	if (end - (void*)new_bl - sizeof(t_block) - new_bl->size > sizeof(t_block))
	{
		set_block_info(tmp, new_bl, new_bl->next,
					end - (void*)tmp - sizeof(t_block));
		tmp->is_free = 1;
		new_bl->next = tmp;
		update_zone(zone, tmp, (sizeof(t_block)));
	}
	else
	{
		new_bl->size += (end - (void*)new_bl - sizeof(t_block) - new_bl->size);
		zone->available -= (end - (void*)new_bl -
							sizeof(t_block) - new_bl->size);
	}
}

t_block	*insert_block(t_zone *zone, size_t size)
{
	t_block	*new_bl;

	new_bl = zone->first_free;
	while (new_bl->next && (new_bl->size < size || !new_bl->is_free))
	{
		new_bl = new_bl->next;
	}
	if (!new_bl->is_free)
	{
		new_bl = append_block(zone, new_bl, size);
		if (!new_bl)
		{
			return (NULL);
		}
	}
	else
	{
		set_block_info(new_bl, new_bl->prev, new_bl->next, size);
		new_bl->is_free = 0;
		update_zone(zone, new_bl, size);
	}
	insert_free_block(new_bl, zone);
	return (new_bl);
}

t_block	*get_block(t_zone *zone, size_t size)
{
	t_block		*block;

	block = NULL;
	if (zone && zone->available >= size + sizeof(t_block))
	{
		block = insert_block(zone, align_block(size));
	}
	if (!zone || !block)
	{
		block = save_big_blocks(size);
	}
	return (block);
}
