/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:39:08 by satushi           #+#    #+#             */
/*   Updated: 2023/03/08 20:36:47 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_node *node, char *path, char **argv, char **environ)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	prepare_pipe_child(node);
	if (redirect_reconect(node->command) == 1)
		exit(1);
	if (is_builtin(node->command->args->word))
		exit(do_builtin(node->command->args->word, node->command));
	else
	{
		if (path[0] == '/' || path[0] == '.')
			execve(path, argv, environ);
		else
		{
			if (searchpath(path) == NULL)
				exit(127);
			else
				execve(searchpath(path), argv, environ);
		}
		exit(127);
	}
}

static void	redirectfile_check(t_redirect *redirect)
{
	int	fd;

	if (redirect->file_path == NULL || redirect->ambigous == true)
		printf("minishell: %s: ambiguous redirect\n", \
		redirect->file_path);
	else
	{
		if (redirect->type == IN)
			fd = open(redirect->file_path, O_RDONLY);
		if (redirect->type == OUT)
			fd = open(redirect->file_path, \
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (redirect->type == APPEND)
			fd = open(redirect->file_path, \
			O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd != -1)
			close (fd);
		write(2, "minishell: ", 12);
		perror(redirect->file_path);
	}
	g_env->err_status = 1;
}

void	exec_check(t_node *node, char *path)
{
	t_redirect	*redirect;
	char		*checked_path;

	redirect = *(node->command->redirect);
	while (redirect != NULL)
	{
		if (redirect->redirectfile == -1 || redirect->ambigous == true)
			return (redirectfile_check(redirect));
		redirect = redirect->next;
	}
	checked_path = searchpath(path);
	if (is_builtin(path) == false && path[0] != '/' && path[0] != '.' \
	&& checked_path == NULL && ft_strcmp("exit", path) != 0)
	{
		printf("minishell: %s: command not found :x\n", path);
		g_env->err_status = 127;
	}
	free(checked_path);
}

pid_t	exec_pipeline(t_node *node)
{
	extern char	**environ;
	pid_t		pid;
	char		**argv;

	argv = args_to_argv(node->command->args);
	if (!argv)
		fatal_error("malloc");
	exec_check(node, argv[0]);
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
		child_process(node, argv[0], argv, environ);
	prepare_pipe_parent(node);
	aray_free(argv);
	if (node->next)
		return (exec_pipeline(node->next));
	return (pid);
}

void	exec(t_node *node)
{
	pid_t	last_pid;

	if (node == NULL)
		last_pid = -1;
	else
	{
		ready_redirectionfile(node);
		last_pid = exec_pipeline(node);
	}
	wait_pipeline(last_pid);
}
