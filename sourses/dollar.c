/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:57:12 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 16:57:15 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*get_var_value(t_env *env_list, char *var_name)
{
	char	*val;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(get_current_exit_status()));
	val = get_env_value(env_list, var_name);
	if (!val)
		return (NULL);
	return (ft_strdup(val));
}

static void	replace_token_var(t_token *token, int dollar_pos, t_env *env_list)
{
	int		var_end;
	char	*var_name;
	char	*new_str;
	char	*var_value;

	var_name = get_var_name(token->str, dollar_pos, &var_end);
	if (!var_name || ft_strlen(var_name) == 0)
	{
		if (var_name)
			free(var_name);
		return ;
	}
	var_value = get_var_value(env_list, var_name);
	new_str = replace_dollar_var(token->str, dollar_pos, var_end, var_value);
	free(var_name);
	if (var_value)
		free(var_value);
	if (new_str)
		token->str = new_str;
}

static void	expand_in_token(t_token *token, t_env *env_list)
{
	int	dollar_pos;

	dollar_pos = 0;
	while ((size_t)dollar_pos < ft_strlen(token->str))
	{
		if (token->str[dollar_pos] == '$')
		{
			replace_token_var(token, dollar_pos, env_list);
			dollar_pos++;
		}
		else
			dollar_pos++;
	}
}

void	expand_dollar_vars(t_token *tokens, int count, t_env *env_list)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if ((tokens[i].type == T_WORD || tokens[i].type == T_IN_FILE
				|| tokens[i].type == T_OUT_FILE
				|| tokens[i].type == T_APPEND_FILE)
			&& tokens[i].quote_type != 1)
			expand_in_token(&tokens[i], env_list);
		i++;
	}
}
