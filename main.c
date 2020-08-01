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

int		main(void)
{
	void	*ptr;

	write(1, "\n", 1);
	ptr = malloc(1);
	show_alloc_mem();
	free(ptr);
	write(1, "\n", 1);
	ptr = malloc(50);
	show_alloc_mem();
	free(ptr);
	write(1, "\n", 1);
	ptr = malloc(500);
	show_alloc_mem();
	free(ptr);
	write(1, "\n", 1);
	ptr = malloc(5000);
	show_alloc_mem();
	free(ptr);
	write(1, "\n", 1);
	ptr = malloc(50000);
	show_alloc_mem();
	free(ptr);
	write(1, "\n", 1);
	return (0);
}
