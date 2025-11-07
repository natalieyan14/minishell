/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/07 16:54:51 by natalieyan       ###   ########.fr       */
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
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		set_exit_status(1);
	}
	else
		set_exit_status(0);
}

int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
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
