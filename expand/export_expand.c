/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 19:43:24 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/03 19:43:24 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	sp_wd_ch(char c)
{
	if (c == '"' || c == '$' || c == '\\')
		return (true);
	return (false);
}

void	export_argsremake_sp(\
size_t position, char **sub, char **dup, char *free_sub)
{
	size_t	last;

	last = ft_strlen(free_sub);
	if (position != last && **sub == '\\' \
	&& sp_wd_ch(**(sub + 1)) == true)
	{
		append_char(&(*dup), **sub);
		(*sub)++;
	}
	else if (*sub != free_sub && *(*sub - 1) != '\\' \
	&& sp_wd_ch(**sub) == true)
		append_char(&(*dup), '\\');
	append_char(&(*dup), **sub);
	(*sub)++;
}

void	export_argsremake(t_token *token)
{
	char	*sub;
	char	*free_sub;
	char	*dup;
	size_t	position;

	position = 0;
	while (token != NULL)
	{
		dup = NULL;
		sub = token->word;
		free_sub = sub;
		while (*sub != '\0' && sub != NULL)
		{
			export_argsremake_sp(position, &sub, &dup, free_sub);
			position++;
		}
		token->word = dup;
		free(free_sub);
		token = token->next;
	}
}

void	append_double_export(char **args, char **new)
{
	while (**args != '\"')
	{
		if (**args == '$' && *(*args + 1) != '\"')
			expand_doller_dq(&(*new), &(*args), *args);
		else
		{
			if (**args == '\\')
			{
				append_char(&(*new), **args);
				(*args)++;
			}
			append_char(&(*new), **args);
			(*args)++;
		}
	}
	(*args)++;
}

char	*expand_args_expote(char *args, char *args_free)
{
	char	*new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\\')
		{
			append_char(&new_word, *args++);
			append_char(&new_word, *args++);
		}
		else if ((*args == '\'' || *args == '\"') && *(args + 1) != '\0')
		{
			args++;
			if (*(args - 1) == '\'')
				append_single(&args, &new_word);
			else if (*(args - 1) == '\"')
				append_double_export(&args, &new_word);
		}
		else if (*args == '$')
			switch_doller_inexpandquote(&new_word, &args);
		else
			append_char(&new_word, *args++);
	}
	free(args_free);
	return (new_word);
}
