/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/02 03:49:47 by natalieyan       ###   ########.fr       */
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

static int	is_numeric(char *str)
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

void	ft_exit(t_command *cmd)
{
	int	status;
	int	arg_count;

	arg_count = 0;
	while (cmd->argc[arg_count])
		arg_count++;
	arg_count--;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	else
		write(1, "minishell> \n", 12);
	if (arg_count == 0)
	{
		exit(0);
	}
	else if (arg_count == 1)
	{
		if (!is_numeric(cmd->argc[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argc[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		status = ft_atoi(cmd->argc[1]);
		exit(status);
	}
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		set_exit_status(1);
		return ;
	}
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

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_variable(t_env **env, char *key, char *val)
{
	t_env	*tmp;
	int		found;

	tmp = *env;
	found = 0;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (val)
			{
				free(tmp->value);
				tmp->value = ft_strdup(val);
			}
			found = 1;
			break ;
		}
		tmp = tmp->next;
	}
	if (!found && val)
	{
		tmp = malloc(sizeof(t_env));
		tmp->key = ft_strdup(key);
		tmp->value = ft_strdup(val);
		tmp->next = *env;
		*env = tmp;
	}
}

static void	print_exported_vars(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value && ft_strlen(tmp->value) > 0)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}

void	ft_export(t_env **env, char **argc)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;
	int		exit_code;

	i = 1;
	exit_code = 0;
	if (!argc[1])
	{
		print_exported_vars(*env);
		set_exit_status(0);
		return ;
	}
	while (argc[i])
	{
		if (!is_valid_identifier(argc[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(argc[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_code = 1;
			i++;
			continue ;
		}
		eq = ft_strchr(argc[i], '=');
		if (eq)
		{
			key = ft_substr(argc[i], 0, eq - argc[i]);
			val = eq + 1;
		}
		else
		{
			key = ft_strdup(argc[i]);
			val = NULL;
		}
		export_variable(env, key, val);
		free(key);
		i++;
	}
	set_exit_status(exit_code);
}
