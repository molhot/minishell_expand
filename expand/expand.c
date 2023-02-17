/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satushi <satushi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:37:31 by satushi           #+#    #+#             */
/*   Updated: 2023/02/18 00:53:40 by satushi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

// void	quote_removal(t_token *tok)
// {
// 	char *new_word;
// 	char *p;

// 	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
// 		return ;
// 	p = tok->word;
// 	new_word = NULL;
// 	while (*p && !is_metacharactert(*p))
// 	{
// 		if (*p == '\'')
// 		{
// 			p++;
// 			while (*p != '\'')
//                 append_char(&new_word, *p++);
// 			// {
// 			// 	if (*p == '\0')
// 			// 		fatal_error("TODO:single quote\n");
// 			// }
// 			p++;
// 		}
// 		else if (*p == '\"')
// 		{
// 			p++;
// 			while (*p != '\"')
//                 append_char(&new_word, *p++);
// 			// {
// 			// 	if (*p == '\0')
// 			// 		fatal_error("TODO:double quote\n");
// 			// 	append_char(&new_word, *p++);
// 			// }
// 			p++;
// 		}
// 		else
// 			append_char(&new_word, *p++);
// 	}
// 	free(tok->word);
// 	tok->word = new_word;
// 	quote_removal(tok->next);
// }

void	expand_doller(char **dst, char **rest, char *p)
{
	char    *name;
    char    *value;

    name = calloc(1, sizeof(char));
    if (name == NULL)
        fatal_error("calloc");
    if (*p != '$')
        fatal_error("Expected dollar sign");
    p++;
    if (!isalpha(*p) && *p != '_')
        fatal_error("Variable must starts with alphabetic character or underscore.");
    append_char(&name,*p++);
    while (isalpha(*p) != 0 || *p == '_' || isdigit(*p) != 0)
        append_char(&name,*p++);
    value = map_get(g_env, name);
    free(name);
    if (value)
        while (*value)
            append_char(dst, *value++);
    *rest = p;
}

void	expand_doller_dq(char **dst, char **rest, char *p)
{
	char    *name;
    char    *value;

    name = calloc(1, sizeof(char));
    if (name == NULL)
        fatal_error("calloc");
    if (*p != '$')
        fatal_error("Expected dollar sign");
    p++;
    if (!isalpha(*p) && *p != '_')
        fatal_error("Variable must starts with alphabetic character or underscore.");
    append_char(&name,*p++);
    while ((isalpha(*p) != 0 || *p == '_' || isdigit(*p) != 0) && *p != '\"')
        append_char(&name,*p++);
    value = getenv(name);
    free(name);
    if (value)
        while (*value)
            append_char(dst, *value++);
    *rest = p;
}

char	*expand_args(char *args, char *args_free)
{
	char *new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\'')
		{
			args++;
			while (*args != '\'')
			{
				if (*args == '\0')
					fatal_error("single quote\n");
				append_char(&new_word, *args++);
			}
			args++;
		}
		else if (*args == '\"')
		{
			args++;
			while (*args != '\"')
			{
				if (*args == '\0')
					fatal_error("double quote\n");
				if (*args == '$')
					expand_doller_dq(&new_word, &args, args);
				else
					append_char(&new_word, *args++);
			}
			args++;
		}
		else if (*args == '$')
			expand_doller(&new_word, &args, args);
		else
			append_char(&new_word, *args++);
	}
	free(args_free);
	return (new_word);
}

void expand(t_node *node)
{
	//char		*origin;
	t_redirect	*redirect;
	t_token		*token;

	//origin = node->command->args->word;
	while (node != NULL)
	{
		token = node->command->args;
		while (token != NULL)
		{
			token->word = expand_args(token->word, token->word);
			token = token->next;
		}
		if (node->command->redirect != NULL)
		{
			redirect = *(node->command->redirect);
			while (redirect != NULL)
			{
				if (redirect->type != HEREDOC)
					redirect->file_path = expand_args(redirect->file_path, redirect->file_path);
				redirect = redirect->next;
			}
		}
		node = node->next;
	}
}