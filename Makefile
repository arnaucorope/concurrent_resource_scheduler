# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acoromin@student.42barcelona.com           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/14 11:10:14 by acoromin          #+#    #+#              #
#    Updated: 2026/09/17 12:51:41 by acoromin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -Iincludes

SRC = \
	src/main.c \
	src/parse/parse.c \
	src/parse/parse_errors.c \
	src/parse/parse_utils.c \
	src/init/init_data.c \
	src/simulation/simulation.c \
	src/simulation/coder.c \
	src/simulation/referee.c \
	src/simulation/stop.c \
	src/simulation/dongle.c \
	src/simulation/dongle_request.c \
	src/simulation/heap.c \
	src/utils/time.c \
	src/utils/dongle_utils.c \
	src/utils/cleanup.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c includes/parse.h includes/codexion.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
