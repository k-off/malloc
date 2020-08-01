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

/*
** *****************************************************************************
** INCLUDES
** *****************************************************************************
*/

# include <unistd.h>
# include <sys/mman.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <pthread.h>
# include <fcntl.h>
# include <stdlib.h>

/*
** *****************************************************************************
** DEFINES
** *****************************************************************************
*/

/*
** type of output for print memory function: short, standard or extended
*/
# define SHORT		0
# define STANDARD	1
# define EXTENDED	2

/*
** block size limits: 0-512 small, 513-10240 medium, >10240 big
*/
# define SMALL_BLOCK  512
# define MEDIUM_BLOCK 10240

/*
** preallocated pages count: 14 small, 264 medium
*/
# define SMALL_ZONE_PAGE_COUNT  14
# define MEDIUM_ZONE_PAGE_COUNT 264

/*
** add or remove block to/from zone
*/
# define ADD     1
# define REMOVE -1

/*
** extra check for blocks
*/
# define MAGIC 371946195

/*
** *****************************************************************************
** STRUCTS
** *****************************************************************************
*/

typedef struct			s_block
{
	struct s_block		*next;
	struct s_block		*prev;
	size_t				size;
	unsigned			is_free;
	unsigned			magic;
}						t_block;

typedef struct			s_zone
{
	t_block				*head;
	t_block				*tail;
	t_block				*first_free;
	size_t				size;
	size_t				available;
}						t_zone;

/*
** *****************************************************************************
** GLOBAL VARIABLES
** *****************************************************************************
*/

/*
** lock
*/
extern pthread_mutex_t	g_mutex;

/*
** zone addresses
*/
extern t_zone			*g_zone[3];

/*
** *****************************************************************************
** FUNCTIONS
** *****************************************************************************
*/

/*
** *****************************************************************************
** block.c - block handling functions
** *****************************************************************************
*/

/*
** @brief Unmap memory area
** @param block pointer to the beginning of memory area
*/
void					munmap_block(t_block *block);

/*
** @brief Set block metadata on allocation or deallocation
** @param block pointer to the block
** @param previous pointer to the block which precedes current one in memory
** @param next pointer to the block which succseeds current one in memory
** @param size total size of block including metadata and data itself
** @return Pointer to the set block
*/
t_block					*set_block_info(t_block *block, t_block *previous,
								t_block *next, size_t size);
/*
** @brief Find a suitable block in a preallocated zone or allocate a new one
** @param zone pointer to the zone
** @param size aligned size of data
** @return pointer to the allocated block or null if block was not found
*/
t_block					*get_block(t_zone *zone, size_t size);

/*
** *****************************************************************************
** malloc.c - main library functions
** *****************************************************************************
*/
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					free(void *ptr);

/*
** *****************************************************************************
** output.c - output functions
** *****************************************************************************
*/

/*
** @brief Print out information about allocated memory zones,
** allocated blocks and total allocated memory
*/
void					show_alloc_mem(void);

/*
** @brief Print out information about allocated memory zones,
**        allocated blocks and total allocated memory
** @param mode print out memory info in a short, standard or extended mode
*/
void					show_alloc_mem_ex(int mode);

/*
** *****************************************************************************
** prints.c - utility functions for output
** *****************************************************************************
*/

/*
** @brief Print a number of type long long recursively with a newline at the end
** @param n number to be printed
** @param is_first must always be set to true on call
*/
void					print_long_long(long long n, int is_first);

/*
** @brief Print a number of type unsigned long long recursively
** @param n number to be printed
** @param is_first must always be set to true on call
*/
void					print_size_t(size_t n, int is_first);

/*
** @brief Print a pointer as a hexadecimal number prefixes with `0x`
** @param ptr address in memory casted to size_t
*/
void					print_pointer(size_t ptr);

/*
** @brief Print out memory in a hexadecimal format 16 bytes per line
** @param prefix string to be printed at the beggining of each line
** @param addr pointer to the beggining of memory area
** @param size number of bytes to be printed
*/
void					print_memory(char *prefix, const void *addr,
									size_t size);

/*
** *****************************************************************************
** utils.c - utility functions for memory allocation
** *****************************************************************************
*/

/*
** @brief Round up `x` to the nearest bigger number which is
** divisible by `alignment`
** @param x number to be aligned
** @param alignment modulo divisor
** @return Aligned value
*/
size_t					align_size(size_t x, size_t alignment);

/*
** @brief Copy `n` bytes of memory from `src` to `dst`
** @param dst pointer to the beginning of destination area in memory
** @param src pointer to the beginning of source area in memory
** @param n amount of bytes to copy
*/
void					*ft_memcpy(void *dst, const void *src, size_t n);

/*
** @brief Get the length of the string
** @param s pointer to the beginning of the string
*/
size_t					ft_strlen(const char *s);

/*
** @brief Copy character array
** @param dest pointer to the beginning of destination character array
** @param src pointer to the beginning of source character array
** @return Pointer to the beginning of destination character array
*/
char					*ft_strcpy(char *dest, const char *src);

/*
** @brief Set each byte of memory to the provided value
** @param s pointer to the beginning of memory area
** @param c value to which memory will be set to
** @param n amount of bytes to be set
** @return Pointer to the beginning of set area
*/
void					*ft_memset(void *s, int c, size_t n);

/*
** *****************************************************************************
** zone.c - zone handling functions
** *****************************************************************************
*/

/*
** @brief Defragment (merge freed) blocks in current memory zone
** @param zone pointer to the beginning of memory zone
** @param block pointer to the block being currently freed
*/
void					defragment(t_zone *zone, t_block *block);

/*
** @brief Update zone metadata depending on action (add or remove block)
** @param zone pointer to zone
** @param block pointer to block
** @param action flag, takes values add(1) or remove(-1)
*/
void					update_zone(t_zone *zone, t_block *block, int action);

/*
** @brief Get pointer to zone according to the requested size
** @param size requested memory size
** @return Pointer to corresponding memory zone
*/
t_zone					*get_zone(size_t size);

#endif
