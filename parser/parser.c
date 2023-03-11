/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 05:32:46 by satushi           #+#    #+#             */
/*   Updated: 2023/03/11 13:43:11 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_node	*ready_nextnode(bool *flag, t_node *node, t_token **token)
{
	if (*flag == true)
		(*(node->command->redirect)) = NULL;
	*flag = true;
	node->next = new_node(ND_SIMPLE_CMD);
	ready_redirectinout(node->next, &(*flag), false);
	node->next->command->redirect = \
	(t_redirect **)malloc(sizeof(t_redirect *) * 1);
	node->next->command->args = NULL;
	*token = (*token)->next;
	return (node->next);
}

void	parse_nullinsert(bool first_action, t_node *node, \
t_redirect *redirection_node)
{
	if (first_action == true)
		(*(node->command->redirect)) = NULL;
	else
		redirection_node->next = NULL;
	node->next = NULL;
}

void	tok_parsing(t_token *tok, t_node *node, bool first_action)
{
	t_redirect	*redirection_node;

	while (tok && !at_eof(tok))
	{
		if (tok->kind == TK_WORD)
			parse_word(&node->command->args, tokdup(tok), &tok);
		else if (tok->kind == TK_REDIRECT)
		{
			if (first_action == true)
				redirection_node = tok_to_redirect_f(&first_action, node, &tok);
			else
				redirection_node = tok_to_redirect(redirection_node, &tok);
			tok = tok->next->next;
		}
		else if (tok->kind == TK_OP)
		{
			if (first_action == false)
				redirection_node->next = NULL;
			node = ready_nextnode(&first_action, node, &tok);
		}
	}
	parse_nullinsert(first_action, node, redirection_node);
}

t_node	*parse(t_token *tok)
{
	t_node		*node;
	t_node		*head;
	bool		first_action;

	node = new_node(ND_SIMPLE_CMD);
	head = node;
	ready_redirectinout(node, &first_action, true);
	tok_parsing(tok, node, first_action);
	return (head);
}
