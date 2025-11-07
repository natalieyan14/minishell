/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:17 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/05 21:27:43 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char	*read_multiline_input(void)
{
	char	*line;
	char	*full_line;
	int		is_interactive;

	// line = readline("");
	line = readline("minishell$ ");
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
		ft_putstr_fd("Error: too many arguments for ./minishell\n", STDERR_FILENO);
		return (3);
	}
	exit_status = 0;
	env_list = init_env(envp);
	exit_status = run_shell(env_list);
	free_env_list(env_list);
	return (exit_status);
}
