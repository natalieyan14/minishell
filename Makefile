NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline #-fsanitize=address

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = sourses/tokenize.c sourses/tokenize_utils.c sourses/tokenize_handlers.c sourses/validation.c \
      sourses/env.c sourses/env_2.c sourses/main.c sourses/pars.c sourses/pars2.c sourses/pars3.c  \
      sourses/pars_cmd.c sourses/pars_redir.c sourses/pars_utils.c \
      sourses/utils.c sourses/utils_2.c sourses/utils_3.c sourses/cmd_exec.c sourses/exec_cmd.c sourses/bulitin.c sourses/bulitin_2.c \
	sourses/dollar.c sourses/dollar_utils.c sourses/heredoc.c sourses/heredoc_utils.c sourses/redirect.c sourses/redirect3.c sourses/redirect4.c sourses/redirect5.c sourses/pipe.c sourses/pipe_utils.c sourses/exec_external.c sourses/exec_single.c sourses/pipe_exec.c sourses/signals.c \
      sourses/signals2.c sourses/exit_status.c sourses/builtin_3.c sourses/builtin_4.c sourses/builtin_utils.c sourses/builtin_basic.c sourses/exec_utils.c sourses/unset_utils.c sourses/export_utils.c \
	  sourses/main_utils1.c sourses/main_utils2.c sourses/main_utils3.c sourses/redirect_helper.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C Libft
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
