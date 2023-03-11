/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doller.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:04:15 by satushi           #+#    #+#             */
/*   Updated: 2023/03/12 03:03:57 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_args_doller(char *args)
{
	char	*new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\\')
		{
			append_char(&new_word, *args++);
			if (*args != '\0')
				append_char(&new_word, *args++);
		}
		else if (*args == '\'' || *args == '\"')
			quote_append_indoller(*args, &new_word, &args);
		else if (*args == '$')
			switch_doller(&new_word, &args);
		else
			append_char(&new_word, *args++);
	}
	return (new_word);
}

void	not_expnad(char **dst, char **rest, char *p)
{
	if (*p == '\\')
	{
		append_char(dst, *(p - 1));
		*rest = p;
		return ;
	}
	if (*p == '\'' || *p == '\"')
	{
		*rest = p;
		return ;
	}
	append_char(dst, *(p - 1));
	append_char(dst, *(p));
	*rest = p + 1;
	return ;
}

void	expand_doller(char **dst, char **rest, char *p)
{
	char	*name;
	char	*value;

	name = calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	p++;
	if (!isalpha(*p) && *p != '_')
	{
		free(name);
		return (not_expnad(&(*dst), &(*rest), p));
	}
	append_char(&name,*p++);
	while (ft_isalpha(*p) != 0 || *p == '_' || ft_isdigit(*p) != 0)
		append_char(&name,*p++);
	value = map_get(g_env, name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	*rest = p;
}

static void	not_expanded(char *name, char **dst, char **rest, char *p)
{
	free(name);
	append_char(dst, *(p - 1));
	if (*p != '\\')
		append_char(dst, *(p));
	*rest = p + 1;
}

void	expand_doller_dq(char **dst, char **rest, char *p)
{
	char	*name;
	char	*value;

	name = calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	p++;
	if (ft_isalpha(*p) != 1 && *p != '_')
		return (not_expanded(name, &(*dst), &(*rest), p));
	append_char(&name,*p++);
	while ((ft_isalpha(*p) != 0 || *p == '_' \
	|| ft_isdigit(*p) != 0) && *p != '\"')
		append_char(&name,*p++);
	value = map_get(g_env, name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	*rest = p;
}
