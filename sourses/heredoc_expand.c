/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:58:28 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/22 22:54:45 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	process_dollar_variable(char **new_line, int i, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	int		var_end;
	int		new_i;

	var_name = get_var_name(*new_line, i, &var_end);
	if (var_name && ft_strlen(var_name) > 0)
	{
		var_value = get_variable_value(var_name, env_list);
		*new_line = replace_dollar_var(*new_line, i, var_end, var_value);
		new_i = i + ft_strlen(var_value);
		free(var_value);
		free(var_name);
		return (new_i);
	}
	if (var_name)
		free(var_name);
	return (i + 1);
}

char	*expand_heredoc_line(char *line, t_env *env_list)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = ft_strdup(line);
	if (!new_line)
		return (NULL);
	while (new_line && (size_t)i < ft_strlen(new_line))
	{
		if (new_line[i] == '$')
			i = process_dollar_variable(&new_line, i, env_list);
		else
			i++;
	}
	return (new_line);
}

int	read_and_process_line(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	char	*str;

	str = readline("> ");
	if (!str)
	{
		handle_eof_warning(limiter);
		return (1);
	}
	if (ft_strcmp(str, limiter) == 0)
	{
		free(str);
		return (1);
	}
	process_heredoc_line(pipe_fd, str, should_expand, env_list);
	free(str);
	return (0);
}

void	run_heredoc(int pipe_fd, char *limiter, int should_expand,
		t_env *env_list)
{
	while (1)
	{
		if (read_and_process_line(pipe_fd, limiter, should_expand, env_list))
			break ;
	}
}
