/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 02:04:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/11/13 15:25:08 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	update_pwd_vars(t_env *env, char *old_pwd, char *new_pwd)
{
	t_env	*pwd_var;
	t_env	*oldpwd_var;

	pwd_var = env;
	while (pwd_var && ft_strcmp(pwd_var->key, "PWD") != 0)
		pwd_var = pwd_var->next;
	if (pwd_var)
	{
		free(pwd_var->value);
		pwd_var->value = ft_strdup(new_pwd);
	}
	oldpwd_var = env;
	while (oldpwd_var && ft_strcmp(oldpwd_var->key, "OLDPWD") != 0)
		oldpwd_var = oldpwd_var->next;
	if (oldpwd_var)
	{
		free(oldpwd_var->value);
		oldpwd_var->value = ft_strdup(old_pwd);
	}
}

static char	*get_cd_path(t_command *cmd, t_env *env, char **oldpwd_ptr)
{
	char	*oldpwd;

	if (!cmd->argc[1])
		return (getenv("HOME"));
	if (cmd->argc[1] && ft_strcmp(cmd->argc[1], "-") == 0)
	{
		oldpwd = get_env_value(env, "OLDPWD");
		if (!oldpwd)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			set_exit_status(1);
			return (NULL);
		}
		*oldpwd_ptr = oldpwd;
		ft_putstr_fd(oldpwd, 2);
		ft_putstr_fd("\n", 2);
		return (oldpwd);
	}
	return (cmd->argc[1]);
}

static void	handle_cd_error(char *path)
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	set_exit_status(1);
}

/* merged into perform_chdir_and_update to reduce function count */

static int	handle_too_many_args(t_command *cmd)
{
	if (cmd && cmd->argc && cmd->argc[1] && cmd->argc[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		set_exit_status(1);
		return (1);
	}
	return (0);
}

void	ft_cd(t_command *cmd, t_env *env)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];

	if (cmd && cmd->argc && cmd->argc[1] && cmd->argc[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		set_exit_status(1);
		return ;
	}
	oldpwd = NULL;
	path = get_cd_path(cmd, env, &oldpwd);
	if (!path)
	{
		if (!cmd->argc[1])
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		set_exit_status(1);
		return ;
	}
    perform_chdir_and_update(env, path, oldpwd, cwd);
}

static void	perform_chdir_and_update(t_env *env, char *path,
		char *oldpwd, char cwd[PATH_MAX])
{
	char	new_cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	if (chdir(path) != 0)
		handle_cd_error(path);
	else
	{
		if (getcwd(new_cwd, sizeof(new_cwd)))
			update_pwd_vars(env, cwd, new_cwd);
		set_exit_status(0);
	}
	if (oldpwd)
		free(oldpwd);
}
