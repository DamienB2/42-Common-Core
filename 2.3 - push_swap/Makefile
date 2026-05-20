NAME = push_swap

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
PRINTF = printf/libftprintf.a

SRC = \
	main.c \
	utils.c \
	utils2.c \
	utils_operations.c \
	utils_op_handler.c \
	insertion_sort.c \
	flag_parser.c \
	index_finder.c \
	chunk_sort.c \
	radix_sort.c \
	strat_selector.c \
	bench.c \
	mini_sort.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

clean:
	rm -f $(OBJ)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	rm -f log.txt
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
