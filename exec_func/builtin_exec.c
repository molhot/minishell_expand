/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:14:01 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/10 19:14:01 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_recover(t_redirect **redirect_array)
{
	t_redirect	*redirect;

	redirect = *redirect_array;
	if (redirect == NULL)
		return ;
	while (redirect->next != NULL)
		redirect = redirect->next;
	while (redirect->before != NULL)
	{
		if (redirect->type == IN || redirect->type == HEREDOC)
			dup2(redirect->stashed_fd, 0);
		else
			dup2(redirect->stashed_fd, 1);
		redirect = redirect->before;
	}
	if (redirect->type == IN || redirect->type == HEREDOC)
		dup2(redirect->stashed_fd, 0);
	else
		dup2(redirect->stashed_fd, 1);
}

void	builtin_exec(t_node *node)
{
	char	**argv;
	size_t	i;

	argv = args_to_argv(node->command->args);
	if (!argv)
		fatal_error("malloc");
	ready_redirectionfile(node);
	exec_check(node, argv[0]);
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	if (redirect_reconect(node->command) == 1)
	{
		g_env->err_status = -1;
		return ;
	}
	g_env->err_status = do_builtin("test", node->command);
	redirect_recover(node->command->redirect);
}
