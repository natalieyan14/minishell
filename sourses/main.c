/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 19:30:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/25 14:16:55 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int			g_exit_status = 0;

static int	handle_input(char *line, t_env *env_list)
{
	t_token		*tokens;
	t_command	*cmd_list;
	char		**env_array;
	int			count;

	count = count_tokens(line);
	tokens = tokenisation(ft_split_input(line), count);
	if (!tokens || if_invalid_input(tokens, count, -1) < 0)
	{
		free(line);
		free_tokens(tokens);
		return (0);
	}
	cmd_list = parse_tokens(tokens, count);
	env_array = list_to_array(env_list);
	exec_cmd_list(cmd_list, env_array);
	free(env_array);
	free_tokens(tokens);
	free_cmd_list(cmd_list);
	free(line);
	return (1);
}

static void	run_shell(t_env *env_list)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (!handle_input(line, env_list))
			continue ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	env_list = init_env(envp);
	run_shell(env_list);
	free_env_list(env_list);
	return (0);
}
