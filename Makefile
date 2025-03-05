# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 21:48:34 by huidris           #+#    #+#              #
#    Updated: 2025/03/06 04:02:55 by huidris          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

FLAGS	= -Wall -Wextra -Werror -pthread #-fsanitize=address -g3 -ggdb

CC	= gcc

SRC_DIR	= src
SRC	= $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR	= obj
OBJS	= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)
	clear

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
