/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:20:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:24:10 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	copy_strings(char *dest, char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	while (s1 && s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
}

char	*ft_strjoin_heredoc(char *s1, char *s2)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1 && !s2)
		return (NULL);
	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	len2 = 0;
	if (s2)
		len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	copy_strings(result, s1, s2);
	if (s1)
		free(s1);
	return (result);
}

int	check_limiter(char *str, char *limiter)
{
	if (!str)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by",
			STDERR_FILENO);
		ft_putstr_fd(" end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(limiter, STDERR_FILENO);
		ft_putstr_fd("')\n", STDERR_FILENO);
		return (1);
	}
	return (ft_strcmp(str, limiter) == 0);
}

char	*get_variable_value(char *var_name, t_env *env_list)
{
	char	*var_value;

	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(get_current_exit_status());
	else
		var_value = get_env_value(env_list, var_name);
	if (!var_value)
		var_value = ft_strdup("");
	return (var_value);
}
