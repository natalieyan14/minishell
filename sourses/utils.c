/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:06:39 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:06:39 by natalieyan       ###   ########.fr       */
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
