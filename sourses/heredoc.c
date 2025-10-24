/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:05:10 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:33 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*ft_strjoin_heredoc(char *s1, char *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*s3;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	s3 = malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (!s3)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		s3[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		s3[i + j] = s2[j];
		j++;
	}
	s3[i + j] = '\0';
	free(s1);
	return (s3);
}

static void	run_heredoc(int pipe_fd, char *limiter)
{
	char	*str;
	char	*line_with_newline;

	while (1)
	{
		str = readline("> ");
		if (!str)
		{
			fprintf(stderr,
				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				limiter);
			break ;
		}
		if (ft_strcmp(str, limiter) == 0)
		{
			free(str);
			break ;
		}
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
		SET_EXIT_STATUS(1);
		return (-1);
	}
	original_stdin = dup(STDIN_FILENO);
	run_heredoc(pipe_fd[1], limiter);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (original_stdin);
}

void	process_heredocs(t_token *tokens, int count)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		if (tokens[i].type == T_HEREDOC && tokens[i + 1].type == T_LIMITER)
		{
			if (handle_heredoc(tokens[i + 1].str) < 0)
			{
				SET_EXIT_STATUS(1);
				return ;
			}
			i += 2;
		}
		else
		{
			i++;
		}
	}
}
