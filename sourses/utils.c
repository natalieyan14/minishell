/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:39 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 21:21:36 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	count_tokens(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
			count++;
		while (line[i] && line[i] != ' ' && line[i] != '\t')
			i++;
	}
	return (count);
}

char	**ft_split_input(char *line)
{
	int		count;
	char	**res;
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	count = count_tokens(line);
	res = malloc(sizeof(char *) * (count + 1));
	start = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		start = i;
		while (line[i] && line[i] != ' ' && line[i] != '\t')
			i++;
		if (i > start)
			res[j++] = ft_substr(line, start, i - start);
	}
	res[j] = NULL;
	return (res);
}

void	free_tokens(t_token *tokens, int count)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < count)
	{
		if (tokens[i].str)
			free(tokens[i].str);
		i++;
	}
	free(tokens);
}

int	err_message(const char *m1, const char *m2, const char *m3)
{
	if (m1)
		fputs(m1, stderr);
	if (m2)
		fputs(m2, stderr);
	if (m3)
		fputs(m3, stderr);
	fputc('\n', stderr);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char	*find_executable_in_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_string_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_string_array(paths);
	return (NULL);
}

void	process_heredocs(t_token *tokens, int count)
{
	int i;

	i = 0;
	while (i < count - 1)
	{
		if (tokens[i].type == T_HEREDOC && tokens[i + 1].type == T_LIMITER)
		{
			if (handle_heredoc(tokens[i + 1].str) < 0)
			{
				set_exit_status(1);
				return ;
			}
			i += 2;
		}
		else
			i++;
	}
}