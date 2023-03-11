/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doller.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kazuki <kazuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:04:15 by satushi           #+#    #+#             */
/*   Updated: 2023/03/11 19:26:53 by kazuki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	not_expand(char **dst, char **rest, char *p)
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
		return (not_expand(&(*dst), &(*rest), p));
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
