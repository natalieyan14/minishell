NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = sources/tokenize/tokenize.c sources/tokenize/tokenize_utils.c sources/tokenize/tokenize_handlers.c sources/pars/validation.c \
      sources/pars/env.c sources/pars/env_2.c sources/main/main.c sources/pars/pars.c sources/pars/pars2.c sources/pars/pars3.c  \
      sources/pars/pars_cmd.c sources/pars/pars_redir.c sources/pars/pars_utils.c \
      sources/utils/utils.c sources/utils/utils_2.c sources/utils/utils_3.c sources/pars/cmd_exec.c sources/pars/exec_cmd.c sources/bultin/bulitin.c sources/bultin/bulitin_2.c \
      sources/bultin/dollar.c sources/bultin/dollar_utils.c sources/redir/heredoc.c sources/redir/redirect.c sources/redir/redirect2.c sources/redir/redirect3.c sources/redir/redirect4.c sources/redir/redirect5.c sources/pipe/pipe.c sources/signals/signals.c \
      sources/signals/signals2.c sources/pars/exit_status.c sources/bultin/builtin_3.c sources/bultin/builtin_4.c \
	  sources/main/main_utils1.c sources/main/main_utils2.c sources/main/main_utils3.c

OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I. -I$(LIBFT_DIR) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
