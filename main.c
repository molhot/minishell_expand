/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:27:43 by satushi           #+#    #+#             */
/*   Updated: 2023/03/11 02:11:30 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_map				*g_env;

static void	exec_switching(t_node *node)
{
	if (node->command->args == NULL && node->command->redirect != NULL)
		ready_redirectionfile(node);
	else if (node->next == NULL && is_builtin(node->command->args->word))
		builtin_exec(node);
	else if (node->command->args->word != NULL)
		exec(node);
}

static void	readline_execpart(char *line)
{
	t_token		*tok;
	t_node		*node;

	signal(SIGINT, SIG_IGN);
	tok = tokenizer(line);
	if (tokcheck(tok) == false || tokwdcheck(tok) == false)
	{
		free_token(tok);
		return ;
	}
	node = parse(tok);
	expand(node);
	exec_switching(node);
	if (tok != NULL)
		free_token(tok);
	if (node != NULL)
		free_node(node);
}

static void	exit_f(void)
{
	//printf("exit\n");
	exit(0);
}

int	main(void)
{
	char		*line;

	rl_outstream = stderr;
	env_init(&g_env);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		line = readline("minishell$ ");
		if (line == NULL)
			exit_f();
		if (*line != 0)
		{
			if (*line)
				add_history(line);
			readline_execpart(line);
		}
		free(line);
	}
}
