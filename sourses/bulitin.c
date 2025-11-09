/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 21:10:59 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	if (str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(char **argc)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argc[i] && is_n_flag(argc[i]))
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
	set_exit_status(0);
}

void	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
	}
	else
		perror("pwd");
	set_exit_status(0);
}

void	ft_cd(t_command *cmd, t_env *env)
{
	char	*path;

	(void)env;
	if (!cmd->argc[1])
		path = getenv("HOME");
	else
		path = cmd->argc[1];
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		set_exit_status(1);
	}
	else if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		set_exit_status(1);
	}
	else
		set_exit_status(0);
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
	set_exit_status(0);
}
