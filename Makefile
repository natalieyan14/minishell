NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = sourses/tokenize.c sourses/env.c sourses/env_2.c sourses/main.c sourses/pars.c \
      sourses/utils.c sourses/cmd_exec.c sourses/exec_cmd.c sourses/bulitin.c sourses/bulitin_2.c

OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

sourses/%.o: sourses/%.c
	$(CC) $(CFLAGS) -I. -I$(LIBFT_DIR) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
