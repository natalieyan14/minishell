/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/25 03:41:50 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_echo(char **argc)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (argc[i] && !ft_strcmp(argc[i], "-n"))
	{
		n_flag = 1;
		i++;
	}
	while (argc[i])
	{
		ft_putstr_fd(argc[i], 1);
		if (argc[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	SET_EXIT_STATUS(0);
}

void	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	SET_EXIT_STATUS(0);
}

void	ft_cd(t_command *cmd, t_env *env)
{
	char	*path;

	(void)env;
	if (!cmd->argc[1])
		path = getenv("HOME");
	else
		path = cmd->argc[1];
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		SET_EXIT_STATUS(1);
	}
	else
		SET_EXIT_STATUS(0);
}

void	ft_exit(t_command *cmd)
{
	int	status;

	status = 0;
	if (cmd->argc[1])
		status = ft_atoi(cmd->argc[1]);
	printf("exit\n");
	exit(status);
}

void	ft_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	SET_EXIT_STATUS(0);
}

void	ft_export(t_env **env, char **argc)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;
	t_env	*tmp;
	int		found;

	i = 1;
	while (argc[i])
	{
		eq = ft_strchr(argc[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		key = ft_substr(argc[i], 0, eq - argc[i]);
		val = ft_strdup(eq + 1);
		tmp = *env;
		found = 0;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, key))
			{
				free(tmp->value);
				tmp->value = val;
				found = 1;
				break ;
			}
			tmp = tmp->next;
		}
		if (!found)
		{
			tmp = malloc(sizeof(t_env));
			tmp->key = key;
			tmp->value = val;
			tmp->next = *env;
			*env = tmp;
		}
		else
			free(key);
		i++;
	}
	SET_EXIT_STATUS(0);
}
