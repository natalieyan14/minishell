/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:39 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 02:57:31 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				return (ft_strdup(current->value));
			else
				return (ft_strdup(""));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}

static char	*get_var_name(char *str, int start, int *end)
{
	int		i;
	char	*var_name;
	int		len;

	i = start + 1;
	if (str[i] == '?')
	{
		*end = i + 1;
		return (ft_strdup("?"));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	len = i - start - 1;
	if (len == 0)
		return (ft_strdup(""));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_memcpy(var_name, str + start + 1, len);
	var_name[len] = '\0';
	return (var_name);
}

static char	*replace_dollar_var(char *str, int start, int end,
		char *replacement)
{
	int		old_len;
	int		new_len;
	int		repl_len;
	char	*result;

	if (!replacement)
		replacement = "";
	old_len = ft_strlen(str);
	repl_len = ft_strlen(replacement);
	new_len = old_len - (end - start) + repl_len;
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, str, start);
	ft_memcpy(result + start, replacement, repl_len);
	ft_memcpy(result + start + repl_len, str + end, old_len - end);
	result[new_len] = '\0';
	return (result);
}

static int	find_dollar_outside_quotes(char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '$' && !in_single_quote)
			return (i);
		i++;
	}
	return (-1);
}

void	expand_dollar_vars(t_token *tokens, int count, t_env *env_list)
{
	int		i;
	int		dollar_pos;
	int		var_end;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	i = 0;
	while (i < count)
	{
		if ((tokens[i].type == T_WORD || tokens[i].type == T_IN_FILE
				|| tokens[i].type == T_OUT_FILE
				|| tokens[i].type == T_APPEND_FILE)
			&& tokens[i].quote_type != 1)
		{
			while ((dollar_pos = find_dollar_outside_quotes(tokens[i].str)) !=
				-1)
			{
				var_name = get_var_name(tokens[i].str, dollar_pos, &var_end);
				if (!var_name)
					break ;
				if (ft_strlen(var_name) == 0)
				{
					free(var_name);
					break ;
				}
				if (ft_strcmp(var_name, "?") == 0)
				{
					var_value = ft_itoa(GET_EXIT_STATUS());
				}
				else
					var_value = get_env_value(env_list, var_name);
				new_str = replace_dollar_var(tokens[i].str, dollar_pos, var_end,
						var_value);
				free(tokens[i].str);
				free(var_name);
				if (var_value)
					free(var_value);
				if (!new_str)
					break ;
				tokens[i].str = new_str;
			}
		}
		i++;
	}
}
