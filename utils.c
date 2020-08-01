/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

size_t	align_size(size_t initial_size, size_t alignment_step)
{
	if (initial_size < 2)
		initial_size = 2;
	return (((initial_size - 1) / alignment_step + 1) * alignment_step);
}

char	*ft_strcpy(char *destination, const char *source)
{
	int		i;

	i = 0;
	while (source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = source[i];
	return (destination);
}

void	*ft_memcpy(void *destination, const void *source, size_t size)
{
	unsigned char	*temporary_destination;
	unsigned char	*temporary_source;
	size_t			i;

	if (destination == NULL && source == NULL)
		return (0);
	i = 0;
	temporary_destination = destination;
	temporary_source = (unsigned char*)source;
	while (i < size)
	{
		temporary_destination[i] = (unsigned char)(temporary_source[i] + '\0');
		i++;
	}
	return (destination);
}

size_t	ft_strlen(const char *string)
{
	size_t	i;

	i = 0;
	while (string[i] != '\0')
		i++;
	return (i);
}

void	*ft_memset(void *source, int value, size_t size)
{
	unsigned char	*pointer;

	pointer = (unsigned char*)source;
	while (size > 0)
	{
		size--;
		pointer[size] = value;
	}
	return (source);
}
