/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:19:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/29 21:21:21 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	copy_strings(char *dest, char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
}

static char	*ft_strjoin_heredoc(char *s1, char *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*s3;

	if (!s1 || !s2)
		return (NULL);
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	s3 = malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (!s3)
		return (NULL);
	copy_strings(s3, s1, s2);
	free(s1);
	return (s3);
}

static int	check_limiter(char *str, char *limiter)
{
	if (!str)
	{
		write(STDERR_FILENO,
			"minishell: warning: here-document delimited by end-of-file\n", 60);
		return (1);
	}
	if (ft_strcmp(str, limiter) == 0)
	{
		free(str);
		return (1);
	}
	return (0);
}

static void	run_heredoc(int pipe_fd, char *limiter)
{
	char	*str;
	char	*line_with_newline;

	while (1)
	{
		str = readline("> ");
		if (check_limiter(str, limiter))
			break ;
		line_with_newline = ft_strjoin_heredoc(ft_strdup(str), "\n");
		if (line_with_newline)
		{
			write(pipe_fd, line_with_newline, ft_strlen(line_with_newline));
			free(line_with_newline);
		}
		free(str);
	}
}

int	handle_heredoc(char *limiter)
{
	int	pipe_fd[2];
	int	original_stdin;

	if (!limiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe error");
		set_exit_status(1);
		return (-1);
	}
	original_stdin = dup(STDIN_FILENO);
	run_heredoc(pipe_fd[1], limiter);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (original_stdin);
}
