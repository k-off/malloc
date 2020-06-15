#******************************************************************************#
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali <pacovali@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/06/04 11:11:47 by pacovali      #+#    #+#                  #
#    Updated: 2020/06/10 19:16:26 by pacovali      ########   odam.nl          #
#                                                                              #
#******************************************************************************#

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SOURCES = malloc.c block.c free.c output.c prints.c utils.c zone.c

OBJECTS = malloc.o block.o free.o output.o prints.o utils.o zone.o


LIB_NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror -fPIC
FLAGS_LIB = -shared

all: $(LIB_NAME)

$(LIB_NAME):
	@$(CC) -c $(FLAGS_CC) $(SOURCES) -O0 -g -I.
	@$(CC) $(FLAGS_LIB) -o $(LIB_NAME) $(OBJECTS)
	@ln -fs $(LIB_NAME) $(LINK)

clean:
	@rm -f *.o .DS_Store *~

fclean: clean
	@rm -f $(LIB_NAME) $(LINK)

re: fclean $(LIB_NAME)