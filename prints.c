/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prints.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/22 16:34:58 by pacovali      #+#    #+#                 */
/*   Updated: 2019/04/22 16:34:58 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	print_memory(char *prefix, const void *addr, size_t size)
{
	unsigned char	*v;
	char			*hex;
	unsigned		i;
	static char		ret[57];

	ft_memset(ret, ' ', 57);
	ret[56] = '\n';
	while (size > 0)
	{
		prefix ? write(1, prefix, ft_strlen(prefix)) : write(1, "", 0);
		v = (unsigned char*)addr;
		hex = "0123456789abcdef";
		i = 0;
		while (i < 16 && i < size)
		{
			ret[i * 2 + i / 2] = hex[v[i] / 16];
			ret[i * 2 + i / 2 + 1] = hex[v[i] % 16];
			ret[40 + i] = (v[i] > 31 && v[i] < 127) ? v[i] : '.';
			i++;
		}
		write(1, ret, 57);
		size -= 16;
		addr += 16;
	}
}

void	print_pointer(size_t ptr)
{
	static char str[11] = "0x100000000";
	static char	*hex = "0123456789abcdef";
	int			i;

	if (!ptr)
		write(1, "0x0", 3);
	i = 8;
	while (i)
	{
		i--;
		str[3 + i] = hex[ptr % 16];
		ptr = ptr >> 4;
	}
	write(1, str, 11);
}

void	print_size_t(size_t n, int is_first)
{
	char	c;

	if (n)
		print_size_t(n / 10, 0);
	c = n % 10 + '0';
	if (n || is_first)
		write(1, &c, 1);
}
