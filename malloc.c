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

void	*malloc(size_t size)
{
	void	*ptr;
	t_block	*block;

	ptr = NULL;
	size = align_block(size);
	block = get_block(get_zone(size), size);
	if (block)
	{
		ptr = (void*)(block + 1);
	}
	return (ptr);
}

void	*realloc(void *ptr, size_t size)
{
	t_block *block;
	void	*new_ptr;

	if (!ptr)
	{
		return (NULL);
	}
	block = (t_block*)ptr - 1;
	if (block->magic != MAGIC || block->is_free)
	{
		return (NULL);
	}
	new_ptr = ptr;
	if (size <= block->size)
	{
		return (new_ptr);
	}
	else
	{
		new_ptr = malloc(size);
		ft_memcpy(new_ptr, ptr, block->size);
		free(ptr);
	}
	return (new_ptr);
}

void	*calloc(size_t nmemb, size_t size)
{
	size_t	*ptr;

	size *= nmemb;
	ptr = (size_t*)malloc(size);
	size /= sizeof(size_t);
	while (size)
	{
		size--;
		ptr[size] = 0;
	}
	return ((void*)ptr);
}
