/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:57:02 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 16:57:05 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char	*get_env_value(t_env *env_list, char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, var_name) == 0)
		{
			if (env_list->value)
				return (ft_strdup(env_list->value));
			return (ft_strdup(""));
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_variable_value(char *var_name, t_env *env_list)
{
	char	*val;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(get_current_exit_status()));
	val = get_env_value(env_list, var_name);
	if (!val)
		return (ft_strdup(""));
	return (val);
}

char	*get_var_name(char *str, int start, int *end)
{
	int		i;
	char	*var_name;

	if (str[start] != '$')
		return (NULL);
	i = start + 1;
	if (str[i] == '?')
	{
		*end = i + 1;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(str[i]) && str[i] != '_')
	{
		*end = start + 1;
		return (ft_strdup(""));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	var_name = ft_substr(str, start + 1, i - start - 1);
	return (var_name);
}

static char	*create_result_string(char *before, char *replacement, char *after)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(before, replacement);
	free(before);
	if (!temp)
	{
		free(after);
		return (NULL);
	}
	result = ft_strjoin(temp, after);
	free(temp);
	free(after);
	return (result);
}

char	*replace_dollar_var(char *str, int start, int end, char *replacement)
{
	char	*before;
	char	*after;
	char	*res;

	if (!replacement)
		replacement = "";
	before = ft_substr(str, 0, start);
	if (!before)
	{
		free(str);
		return (NULL);
	}
	after = ft_strdup(str + end);
	if (!after)
	{
		free(before);
		free(str);
		return (NULL);
	}
	{
		res = create_result_string(before, replacement, after);
		free(str);
		return (res);
	}
}
