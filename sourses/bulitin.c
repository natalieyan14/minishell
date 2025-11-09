/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/09 16:56:40 by natalieyan       ###   ########.fr       */
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

static int	has_valid_format(char *str, int *i)
{
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '+' && *(str + 1) == '+')
			return (0);
		if (str[*i] == '-' && *(str + 1) == '-')
			return (0);
		(*i)++;
	}
	if (!str[*i])
		return (0);
	return (1);
}

static int	check_overflow(char *str, int i, int sign)
{
	long	result;

	result = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (sign == 1)
		{
			if (result > LONG_MAX / 10 || (result == LONG_MAX / 10 && str[i]
					- '0' > LONG_MAX % 10))
				return (0);
		}
		else
		{
			if (result > -(LONG_MIN / 10) || (result == -(LONG_MIN / 10)
					&& str[i] - '0' > -(LONG_MIN % 10)))
				return (0);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

int	is_numeric(char *str)
{
	int	i;
	int	sign;

	if (!str || !*str)
		return (0);
	i = 0;
	sign = 1;
	if (!has_valid_format(str, &i))
		return (0);
	if (str[0] == '-')
		sign = -1;
	if (str[0] == '+' || str[0] == '-')
		return (check_overflow(str, 1, sign));
	else
		return (check_overflow(str, 0, sign));
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
