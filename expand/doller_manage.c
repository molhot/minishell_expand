/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doller_manage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 09:58:40 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/10 09:58:40 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	switch_doller_inexpandquote(char **new_word, char **args)
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
