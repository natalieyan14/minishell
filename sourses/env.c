/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:59:50 by natalieyan        #+#    #+#             */
/*   Updated: 2025/09/25 13:42:32 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	ft_env_lstsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	*ft_strjoin_env(const char *s1, const char *s2, char sep)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 2);
	if (!res)
		return (NULL);
	memcpy(res, s1, len1);
	res[len1] = sep;
	memcpy(res + len1 + 1, s2, len2);
	res[len1 + len2 + 1] = '\0';
	return (res);
}

char	**env_to_array(t_env *env_list)
{
	int		len;
	char	**env;
	int		i;

	len = ft_env_lstsize(env_list);
	env = malloc(sizeof(char *) * (len + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (env_list)
	{
		env[i] = ft_strjoin_env(env_list->key, env_list->value, '=');
		env_list = env_list->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

void	print_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	add_env_node(t_env **env_list, char *env_line)
{
	t_env	*new;
	t_env	*tmp;
	char	*eq;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	eq = ft_strchr(env_line, '=');
	if (!eq)
	{
		free(new);
		return ;
	}
	new->key = ft_substr(env_line, 0, eq - env_line);
	new->value = ft_strdup(eq + 1);
	new->next = NULL;
	if (!*env_list)
		*env_list = new;
	else
	{
		tmp = *env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		add_env_node(&env_list, envp[i]);
		i++;
	}
	return (env_list);
}

char	**list_to_array(t_env *env_list)
{
	return (env_to_array(env_list));
}
