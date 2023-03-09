/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kazuki <kazuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 20:18:23 by user              #+#    #+#             */
/*   Updated: 2023/03/10 01:58:04 by kazuki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	show_manual(char **commands)
{
	free_commands(commands);
	ft_putendl_fd("cd with only a relative or absolute path", 2);
	return ;
}

static	bool	is_sym(char *path)
{
	struct stat	st;

	if (lstat(path, &st) == 0)
	{
		if (S_ISLNK(st.st_mode) != 0)
			return (true);
	}
	return (false);
}

void	set_symlink(char *path)
{
	char	*old;
	char	*tmp;
	char	*trim_path;
	char	*new;

	old = map_get(g_env, "PWD");
	tmp = ft_strjoin(old, "/");
	if (!tmp)
		fatal_error("malloc");
	if (ft_strrchr(path, '/'))
		trim_path = ft_strrchr(path, '/') + 1;
	else
		trim_path = path;
	new = ft_strjoin(tmp, trim_path);
	if (!new)
		fatal_error("malloc");
	free(tmp);
	if (ft_strcmp(ft_strrchr(old, '/') + 1, trim_path) != 0)
		map_set(&g_env, "PWD", new);
	free(new);
}

static	void	free_all(char *cwd, char **commands)
{
	free(cwd);
	free_commands(commands);
}

void	ms_cd(t_command *command)
{
	char	**commands;
	char	buf[256];
	char	*cwd;

	commands = command_to_array(command);
	if (!commands)
		fatal_error("malloc");
	if (commands[1] == NULL || commands[2] != NULL \
	|| ft_strchr(commands[1], '~'))
	{
		show_manual(commands);
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (chdir(commands[1]) < 0 || cwd == NULL)
	{
		perror("chdir");
		free_all(cwd, commands);
		return ;
	}
	if (is_sym(commands[1]))
		set_symlink(commands[1]);
	else
		map_set(&g_env, "PWD", getcwd(buf, sizeof(buf)));
	free_all(cwd, commands);
}
