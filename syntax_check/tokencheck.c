/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokencheck.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 18:01:14 by satushi           #+#    #+#             */
/*   Updated: 2023/03/08 19:19:30 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	only_twowd(char	*str)
{
	if (*str == '\\' && (*(str + 1) == '\"' || \
	*(str + 1) == '\'') && *(str + 2) == '\0')
		return (true);
	return (false);
}

size_t	not_escaped(char *args)
{
	size_t	position;

	position = 0;
	while (args[position] != '\0')
	{
		if (args[position] == '\'' || args[position] == '\"')
			if (position != 0 && args[position - 1] != '\\')
				return (true);
		args++;
	}
	return (false);
}

static bool	show_error(char *s, size_t charlen)
{
	if (s[charlen - 1] == '\\')
	{
		printf("bash: syntax error near newline `%s'\n", "\\n");
		return (false);
	}
	printf("bash: syntax error near quote `%s'\n", s);
	return (false);
}

bool	wdcheck(char **str)
{
	char	type;
	char	*tmp_str;

	tmp_str = *str;
	if (tmp_str[ft_strlen(*str) - 1] == '\\')
		return (show_error(tmp_str, ft_strlen(tmp_str)));
	if ((ft_strchr(*str, '\'') != NULL || ft_strchr(*str, '\"') != NULL) \
	&& not_escaped(*str) == true)
	{
		while (**str != '\'' && **str != '\"')
			(*str)++;
		type = **str;
		(*str)++;
		while (**str != type && **str != '\0')
		{
			if (**str == '\\')
				(*str)++;
			(*str)++;
		}
		if (**str == '\0')
			return (show_error(tmp_str, ft_strlen(tmp_str)));
	}
	return (true);
}

bool	tokwdcheck(t_token *tok)
{
	char	*str;

	while (tok != NULL && tok->kind != TK_EOF)
	{
		str = tok->word;
		if (only_twowd(str) != true)
		{
			while (*str != '\0')
			{
				if (false == wdcheck(&str))
					return (false);
				str++;
			}
		}
		tok = tok->next;
	}
	return (true);
}
