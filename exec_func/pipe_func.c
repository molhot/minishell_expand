/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:16:50 by satushi           #+#    #+#             */
/*   Updated: 2023/03/08 19:04:32 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	cpy_pipe(int dst[2], int src[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}

void	prepare_pipe(t_node *node)
{
	if (node->next == NULL)
		return ;
	if (pipe(node->command->out_fd) < 0)
		fatal_error("pipe");
	cpy_pipe(node->next->command->in_fd, node->command->out_fd);
}

void	prepare_pipe_child(t_node *node)
{
	if (node->command->out_fd[0] != -1)
		close(node->command->out_fd[0]);
	dup2(node->command->in_fd[0], STDIN_FILENO);
	if (node->command->in_fd[0] != STDIN_FILENO)
		close(node->command->in_fd[0]);
	dup2(node->command->out_fd[1], STDOUT_FILENO);
	if (node->command->out_fd[1] != STDOUT_FILENO)
		close(node->command->out_fd[1]);
}

void	prepare_pipe_parent(t_node *node)
{
	if (node->command->in_fd[0] != STDIN_FILENO)
		close(node->command->in_fd[0]);
	if (node->next != NULL)
		close(node->command->out_fd[1]);
}

void	wait_pipeline(pid_t last_pid)
{
	int		wstatus;
	bool	last;

	last = false;
	while (1)
	{
		if (wait(&wstatus) == last_pid)
			last = true;
		else if (WIFSIGNALED(wstatus))
		{
			if (WTERMSIG(wstatus) == SIGQUIT && !last)
				ft_putendl_fd("Quit: 3", STDERR_FILENO);
			else if (WTERMSIG(wstatus) == SIGQUIT && last)
				ft_putstr_fd("", STDERR_FILENO);
			if (WTERMSIG(wstatus) == SIGINT)
				ft_putendl_fd("", STDERR_FILENO);
			g_env->err_status = WTERMSIG(wstatus) + 128;
			break ;
		}
		else if (last)
			break ;
	}
	if (last && WIFSIGNALED(wstatus))
		g_env->err_status = WEXITSTATUS(wstatus);
}
