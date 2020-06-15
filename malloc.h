/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   malloc.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/01 00:00:42 by pacovali      #+#    #+#                 */
/*   Updated: 2019/01/01 00:01:42 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MALLOC_H
# define __MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <sys/time.h>
# include <sys/resource.h>

# define MAGIC   1234567890

# define SIZE_S  512
# define SIZE_M  10240

# define PAGES_S 16
# define PAGES_M 264

# define S 0
# define M 1

# define SHRT 0
# define STD 1
# define EXT 2

typedef struct		s_block
{
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	int				is_free;
	int				magic;
}					t_block;

typedef struct		s_zone
{
	void			*begin;
	void			*end;
	t_block			*head;
	t_block			*tail;
	t_block			*first_free;
	size_t			size;
	size_t			available;
	size_t			type;
}					t_zone;

size_t				align_block(size_t x);
size_t				align_page(size_t x);

void				set_zone(t_zone *zone, size_t size, size_t type);
t_zone				*get_zone(size_t size);
void				update_zone(t_zone *zone, t_block *tmp, size_t size);
t_zone				*is_in_zone(void *block);

void				set_block_info(t_block *block, t_block *p, t_block *n,
								size_t size);
t_block				*append_block(t_zone *zone, t_block *new_bl, size_t size);
void				insert_free_block(t_block *new_bl, t_zone *zone);
t_block				*insert_block(t_zone *zone, size_t size);
t_block				*get_block(t_zone *zone, size_t size);

void				show_alloc_mem(void);
void				show_alloc_mem_ex(int mode);

void				print_memory(char *prefix, const void *addr, size_t size);

t_block				*save_big_blocks(size_t size);
void				free_big_blocks(t_block *block);
t_block				*manage_big_list(t_block *add, t_block *rem);

void				free_zone(t_zone *zone, t_block *block);
void				free(void *ptr);
void				*malloc(size_t size);
void				*calloc(size_t nmemb, size_t size);
void				*realloc(void *ptr, size_t size);

void				print_pointer(size_t ptr);
void				print_size_t(size_t n, int is_first);

void				*ft_memcpy(void *dst, const void *src, size_t n);
size_t				ft_strlen(const char *s);
char				*ft_strcpy(char *dest, const char *src);
void				*ft_memset(void *s, int c, size_t n);

#endif
