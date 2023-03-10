/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:37:31 by satushi           #+#    #+#             */
/*   Updated: 2023/03/10 23:47:08 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_args(char *args, char *args_free)
{
	char	*new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\'' || *args == '\"')
		{
			args++;
			if (*(args - 1) == '\'')
				append_single(&args, &new_word);
			else if (*(args - 1) == '\"')
				append_double(&args, &new_word);
			args++;
		}
		else if (*args == '$' && *(args + 1) == '?')
			expand_dolleeques(&new_word, &args, args);
		else if (*args == '$')
			expand_doller(&new_word, &args, args);
		else
			append_char(&new_word, *args++);
	}
	free(args_free);
	return (new_word);
}

void	expand_redirect(t_redirect *redirect)
{
	while (redirect != NULL)
	{
		if (redirect->type != HEREDOC)
			redirect->file_path = expand_args(redirect->file_path, \
			redirect->file_path);
		redirect = redirect->next;
	}
}

void	expand_export(t_token *token)
{
	while (token != NULL)
	{
		if (token->word != NULL)
			token->word = expand_args_expote(token->word, token->word);
		token = token->next;
	}
}

t_token	*expand_simplecommand(t_token *token)
{
	t_token	*re_token;
	t_token	*f_re_tok;
	bool	export_is;

	re_token = (t_token *)malloc(sizeof(t_token) * 1);
	f_re_tok = re_token;
	export_is = false;
	if (ft_strcmp(token->word, "export") == 0)
		export_is = true;
	(void)export_is;
	if (export_is == false)
	{
		expand_specialparam(token);
		remake_token(token, re_token);
		expand_quote(re_token);
		return (f_re_tok);
	}
	else
	{
		free(re_token);
		expand_export(token);
		export_argsremake(token);
		return (token);
	}
}

void	expand(t_node *node)
{
	t_redirect	*redirect;
	t_token		*token;

	while (node != NULL)
	{
		token = node->command->args;
		if (node->command->args != NULL)
			node->command->args = expand_simplecommand(token);
		if (node->command->redirect != NULL)
		{
			redirect = *(node->command->redirect);
			*(node->command->redirect) = expand_redirect_ten(redirect);
		}
		node = node->next;
	}
}
