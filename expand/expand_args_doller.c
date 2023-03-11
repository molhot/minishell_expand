/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_doller.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kazuki <kazuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 19:26:07 by kazuki            #+#    #+#             */
/*   Updated: 2023/03/11 19:27:04 by kazuki           ###   ########.fr       */
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
