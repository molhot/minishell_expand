/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:27:43 by satushi           #+#    #+#             */
/*   Updated: 2023/02/26 14:49:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_map	*g_env;

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

static void	builtin_exec(t_node *node)
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

static void	readline_execpart(char *line)
{
	t_token		*tok;
	t_node		*node;

	tok = tokenizer(line);
	if (tokcheck(tok) == false || tokwdcheck(tok) == false)
	{
		free_token(tok);
		return ;
	}
	node = parse(tok);
	expand(node);
	if (node->command->args == NULL && node->command->redirect != NULL)
		ready_redirectionfile(node);
	else if (node->next == NULL && is_builtin(node->command->args->word))
		builtin_exec(node);
	else
		g_env->err_status = exec(node);
	if (tok != NULL)
		free_token(tok);
	if (node != NULL)
		free_node(node);
}

int	main(void)
{
	char		*line;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	rl_outstream = stderr;
	env_init(&g_env);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line != 0)
		{
			if (*line)
				add_history(line);
			readline_execpart(line);
		}
		free(line);
	}
	exit(0);
}
