/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satushi <satushi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 20:18:23 by user              #+#    #+#             */
/*   Updated: 2023/02/23 17:28:38 by satushi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_cd(char *line, t_command *command)
{
	char	**commands;
	char	*path;
	char	buf[256];
	char	*cwd;

	(void)line;
	commands = command_to_array(command);
	if (!commands)
		fatal_error("malloc");
	if (commands[1] == NULL || commands[2] != NULL || ft_strchr(commands[1], '~'))
	{
		free_commands(commands);
		ft_putendl_fd("cd with only a relative or absolute path", 2);
		return ;
	}
	path = commands[1];
	cwd = getcwd(NULL, 0);
	if (chdir(path) < 0 || cwd == NULL)
	{
		perror("chdir");
		free(cwd);
		free_commands(commands);
		return ;
	}
	map_set(&g_env, "PWD", getcwd(buf, sizeof(buf)));
	free(cwd);
	free_commands(commands);
}
