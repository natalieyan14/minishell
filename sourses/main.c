/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:17 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/11 13:44:18 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*read_initial_line(void)
{
	char		*line;
	static int	first_call = 1;
	char		*buf;
	ssize_t		rd;
	size_t		len;
	char		c;

	rd = 0;
	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
	{
		if (first_call)
		{
			write(1, "minishell$ \n", 12);
			first_call = 0;
		}
		buf = malloc(1);
		if (!buf)
			return (NULL);
		len = 0;
		while ((rd = read(STDIN_FILENO, &c, 1)) > 0)
		{
			if (c == '\n')
				break ;
			buf = realloc(buf, len + 2);
			if (!buf)
				return (NULL);
			buf[len] = c;
			len++;
			buf[len] = '\0';
		}
		if (rd <= 0 && len == 0)
		{
			free(buf);
			return (NULL);
		}
		line = buf;
	}
	return (line);
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
	free_env_list(env_list);
	return (exit_status);
}
