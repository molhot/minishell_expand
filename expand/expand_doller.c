/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doller.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:04:15 by satushi           #+#    #+#             */
/*   Updated: 2023/03/09 22:40:37 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	quote_append(char type, char **new, char **args)
{
	append_char(&(*new), **args);
	(*args)++;
	if (**args == '\0')
		return ;
	while (**args != type)
	{
		append_char(&(*new), **args);
		(*args)++;
	}
	append_char(&(*new), **args);
	(*args)++;
}

// else if (*args == '$' && *(args + 1) == '\0')
// 	append_char(&new_word, *args++);
// else if (*args == '$' && (*(args + 1) == '\'' 
// || *(args + 1) == '\"'))
// 	args++;
// else if (*args == '$' && *(args + 1) == '?')
// 	expand_dolleeques(&new_word, &args, args);
// else if (*args == '$')
// 	expand_doller(&new_word, &args, args);

static void	switch_doller(char **new_word, char **args)
{
	if (**args == '$' && *(*args + 1) == '\0')
	{
		append_char(&(*new_word), **args);
		(*args)++;
	}
	else if (**args == '$' && (*(*args + 1) == '\'' || *(*args + 1) == '\"'))
		(*args)++;
	else if (**args == '$' && *(*args + 1) == '?')
		expand_dolleeques(&(*new_word), &(*args), *args);
	else if (**args == '$')
		expand_doller(&(*new_word), &(*args), *args);
}

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
			quote_append(*args, &new_word, &args);
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
		return (not_expnad(&(*dst), &(*rest), p));
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

void	expand_doller_dq(char **dst, char **rest, char *p)
{
	char	*name;
	char	*value;

	name = calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	p++;
	if (ft_isalpha(*p) != 1 && *p != '_')
	{
		append_char(dst, *(p - 1));
		append_char(dst, *(p));
		*rest = p + 1;
		return ;
	}
	append_char(&name,*p++);
	while ((ft_isalpha(*p) != 0 || *p == '_' \
	|| ft_isdigit(*p) != 0) && *p != '\"')
		append_char(&name,*p++);
	value = getenv(name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	*rest = p;
}
