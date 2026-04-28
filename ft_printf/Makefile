NAME        = libftprintf.a
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

# Chemins vers la libft
LIBFT_PATH  = ../libft
LIBFT       = $(LIBFT_PATH)/libft.a

SRCS        = ft_printf.c utils.c convert.c
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(NAME): $(LIBFT) $(OBJS)
	cp $(LIBFT) $(NAME)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -I$(LIBFT_PATH) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re $(LIBFT)