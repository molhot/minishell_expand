/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kazuki <kazuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:32:50 by satushi           #+#    #+#             */
/*   Updated: 2023/02/21 17:44:37 by kazuki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	heredoc(const char *deli)
{
	char	*line;
	int		pfd[2];

	if (pipe(pfd) < 0)
		return (25555);
	while (1)
	{
		line = readline("input > ");
		if (line == NULL)
			break ;
		else if (strcmp(line, deli) == 0)
		{
			free(line);
			break ;
		}
		dprintf(pfd[1], "%s\n", line);
		free(line);
	}
	close (pfd[1]);
	return (pfd[0]);
}

void	ready_redirectionfile(t_node *node)
{
	int			fd;
	t_redirect	*redirect;

	while (node != NULL)
	{
		redirect = node->command->redirect;
		while (redirect != NULL)
		{
			if (redirect->type == IN)
				fd = open(redirect->file_path, O_RDONLY);
			if (redirect->type == HEREDOC)
				fd = heredoc(redirect->file_path);
			if (redirect->type == OUT)
				fd = open(redirect->file_path, \
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (redirect->type == APPEND)
				fd = open(redirect->file_path, \
				O_CREAT | O_WRONLY | O_APPEND, 0644);
			redirect->redirectfile = fd;
			//redirect->redirectfile = stashfd(fd);
			redirect = redirect->next;
		}
		node = node->next;
	}
}

void	redirect_reconect(t_command *command)
{
	t_redirect	*redirect;

	if (command->redirect == NULL)
		return ;
	redirect = command->redirect;
	while (redirect != NULL)
	{
		if (redirect->type == IN || redirect->type == HEREDOC)
		{
			dup2(redirect->redirectfile, 0);
			close(redirect->redirectfile);
			command->now_in = redirect->redirectfile;
		}
		if (redirect->type == OUT || redirect->type == APPEND)
		{
			dup2(redirect->redirectfile, 1);
			close(redirect->redirectfile);
			command->now_out = redirect->redirectfile;
		}
		redirect = redirect->next;
	}
}
