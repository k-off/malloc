/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prints.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/22 16:34:58 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/26 15:35:13 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	print_size_t(size_t value, int is_first)
{
	char	digit;

	if (value)
		print_size_t(value / 10, 0);
	digit = value % 10 + '0';
	if (value || is_first)
		write(1, &digit, 1);
}

void	print_long_long(long long value, int is_first)
{
	char digit;

	if (value < 0 && is_first)
		write(1, "-", 1);
	else if (value < 0)
		value = -value;
	digit = (value % 10);
	digit = (digit < 0 ? -digit : digit) + '0';
	if (value || is_first)
	{
		print_size_t((value / 10) < 0 ? -(value / 10) : (value / 10), 0);
		write(1, &digit, 1);
	}
}

void	print_memory(char *prefix, const void *address, size_t size)
{
	unsigned char	*value;
	char			*hexadecimal;
	unsigned		i;
	char			print[57];

	ft_memset(print, ' ', 57);
	print[56] = '\n';
	while (size > 0)
	{
		prefix ? write(1, prefix, ft_strlen(prefix)) : write(1, "", 0);
		value = (unsigned char*)address;
		hexadecimal = "0123456789abcdef";
		i = 0;
		while (i < 16 && i < size)
		{
			print[i * 2 + i / 2] = hexadecimal[value[i] / 16];
			print[i * 2 + i / 2 + 1] = hexadecimal[value[i] % 16];
			print[40 + i] = (value[i] > 31 && value[i] < 127) ? value[i] : '.';
			i++;
		}
		write(1, print, 57);
		address += (size > 15 ? 16 : size);
		size -= (size > 15 ? 16 : size);
	}
}

void	print_pointer(size_t pointer)
{
	static char string[11] = "0x100000000";
	static char	*hexadecimal = "0123456789abcdef";
	int			i;

	if (!pointer)
		write(1, "0x0", 3);
	i = 8;
	while (i)
	{
		i--;
		string[3 + i] = hexadecimal[pointer % 16];
		pointer = pointer >> 4;
	}
	write(1, string, 11);
}
