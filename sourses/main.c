/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:17 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 11:11:21 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char		*read_multiline_input(void);

static char	*read_non_interactive_line(void)
{
	char	*buf;
	char	c;
	ssize_t	rd;

	buf = ft_strdup("");
	if (!buf)
		return (NULL);
	rd = read(STDIN_FILENO, &c, 1);
	while (rd > 0 && c != '\n')
	{
		append_char(&buf, c);
		rd = read(STDIN_FILENO, &c, 1);
	}
	if (rd <= 0 && ft_strlen(buf) == 0)
	{
		free(buf);
		return (NULL);
	}
	return (buf);
}

static char	*read_initial_line(void)
{
	char		*line;
	static int	first_call = 1;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
	{
		if (first_call)
		{
			write(1, "minishell$ \n", 12);
			first_call = 0;
		}
		line = read_non_interactive_line();
	}
	return (line);
}

int	run_shell(t_env *env_list)
{
	char	*line;

	setup_interactive_signals();
	while (1)
	{
		line = read_multiline_input();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(1, "exit\n", 5);
			break ;
		}
		if (*line)
		{
			add_history(line);
			handle_input(line, &env_list);
		}
		else
			free(line);
	}
	return (get_current_exit_status());
}

char	*read_multiline_input(void)
{
	char	*line;
	char	*full_line;
	int		is_interactive;

	line = read_initial_line();
	if (!line)
		return (NULL);
	full_line = ft_strdup(line);
	free(line);
	is_interactive = isatty(STDIN_FILENO);
	while (has_unclosed_quotes(full_line))
	{
		full_line = read_continuation_line(full_line, is_interactive);
		if (!full_line)
			return (NULL);
	}
	return (full_line);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	int		exit_status;

	if (argc > 1 && argv)
	{
		ft_putstr_fd("Error: too many arguments for ./minishell\n",
			STDERR_FILENO);
		return (3);
	}
	exit_status = 0;
	env_list = init_env(envp);
	exit_status = run_shell(env_list);
	/* free readline history to avoid reachable allocations in valgrind */
	clear_history();
	free_env_list(env_list);
	return (exit_status);
}
