/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simplecommand.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:31:02 by marvin            #+#    #+#             */
/*   Updated: 2023/02/21 22:31:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_tokenword(t_token **token, t_token **re_token)
{
	char	**tokenwd_split;
	size_t	position;

	tokenwd_split = ft_split((*token)->word, ' ');
	position = 0;
	while (tokenwd_split[position] != NULL)
	{
		(*re_token)->word = ft_strdup(tokenwd_split[position]);
		(*re_token)->kind = TK_WORD;
		if (tokenwd_split[position + 1] != NULL)
		{
			(*re_token)->next = (t_token *)malloc(sizeof(t_token) * 1);
			*re_token = (*re_token)->next;
		}
		free(tokenwd_split[position]);
		position++;
	}
	free(tokenwd_split);
	(*token) = (*token)->next;
}

void	expand_specialparam(t_token *token)
{
	char	*tmp;

	while (token != NULL)
	{
		tmp = ft_strdup(token->word);
		free(token->word);
		token->word = expand_args_doller(tmp);
		free(tmp);
		token = token->next;
	}
}

void	expand_quote(t_token *token)
{
	while (token != NULL)
	{
		if (token->word != NULL)
			token->word = expand_args_quote(token->word, token->word);
		token = token->next;
	}
}

// else if (*args == '$' && (*(args + 1) == '\0' ||
// *(args + 1) == '\'' || *(args + 1) == '\"'))
// 	append_char(&new_word, *args++);
// else if (*args == '$' && *(args + 1) == '?')
// 	expand_dolleeques(&new_word, &args, args);
// else if (*args == '$')
// 	expand_doller(&new_word, &args, args);

void	switch_quote_doller(char **args, char **newword)
{
	char	*tmp;

	tmp = *args;
	if (*tmp == '$' && (*(tmp + 1) == '\0' || \
	*(tmp + 1) == '\'' || *(tmp + 1) == '\"'))
	{
		append_char(&(*newword), *tmp);
		(*args)++;
	}
	else if (*tmp == '$' && *(tmp + 1) == '?')
		expand_dolleeques(&(*newword), &(*args), tmp);
	else if (*tmp == '$')
		expand_doller(&(*newword), &(*args), tmp);
}

char	*expand_args_quote(char *args, char *args_free)
{
	char	*new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\\')
		{
			args++;
			append_char(&new_word, *args++);
		}
		else if ((*args == '\'' || *args == '\"') && *(args + 1) != '\0')
		{
			args++;
			if (*(args - 1) == '\'')
				append_single(&args, &new_word);
			else if (*(args - 1) == '\"')
				append_double(&args, &new_word);
		}
		else if (*args == '$')
			switch_quote_doller(&args, &new_word);
		else
			append_char(&new_word, *args++);
	}
	free(args_free);
	return (new_word);
}
