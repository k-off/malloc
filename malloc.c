/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   malloc.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t		g_mutex;
t_zone				*g_zone[3] = {NULL, NULL, NULL};

static int			is_in_zone(void *pointer, t_block **block)
{
	int			i;

	i = 0;
	if (pointer == NULL || (size_t)pointer % sizeof(size_t) ||
		(size_t)pointer - (size_t)g_zone[0]->head > 0xffffffffu)
		i = -1;
	else
	{
		*block = (t_block*)pointer;
		(*block)--;
		if (block[0]->magic != MAGIC || block[0]->is_free)
			i = -1;
	}
	while (i > -1 && i < 3)
	{
		if (g_zone[i] && pointer >= (void*)(g_zone[i]->head) &&
			pointer <= (void*)((g_zone[i]->tail) + 1))
			return (i);
		i++;
	}
	pthread_mutex_unlock(&g_mutex);
	return (i);
}

void				free(void *ptr)
{
	t_block		*block;
	int			zone_index;

	pthread_mutex_lock(&g_mutex);
	zone_index = is_in_zone(ptr, &block);
	if (zone_index < 0)
		return ;
	update_zone(g_zone[zone_index], block, REMOVE);
	block->is_free = 1;
	block->magic = 0;
	if (zone_index < 2)
		defragment(g_zone[zone_index], block);
	else
		munmap_block(block);
	pthread_mutex_unlock(&g_mutex);
}

void				*realloc(void *ptr, size_t size)
{
	t_block *block;
	void	*new_ptr;

	if (!ptr && size)
		return (malloc(size));
	if (is_in_zone(ptr, &block) < 0)
		return (NULL);
	new_ptr = ptr;
	if (size <= block->size)
		return (new_ptr);
	else
	{
		new_ptr = malloc(size);
		ft_memcpy(new_ptr, ptr, block->size);
		free(ptr);
	}
	return (new_ptr);
}

void				*malloc(size_t size)
{
	t_block		*block;
	void		*ptr;

	pthread_mutex_lock(&g_mutex);
	ptr = NULL;
	block = NULL;
	size = align_size(size, sizeof(t_block));
	block = get_block(get_zone(size), size);
	if (!block)
		block = get_block(get_zone(MEDIUM_BLOCK + 1), size);
	if (block)
		ptr = (void*)(++block);
	block--;
	pthread_mutex_unlock(&g_mutex);
	return (ptr);
}
