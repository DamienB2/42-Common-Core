NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRC = \
	codexion.c \
	parser.c \
	parser_helper.c \
	time_helper.c \
	simulation.c \
	priority_queue.c \
	dongle.c \
	dongle_init.c \
	sim_utils.c	\
	sim_utils_2.c \
	coder.c \
	coder_actions.c \
	sim_init.c \
	monitor.c

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c codexion.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re