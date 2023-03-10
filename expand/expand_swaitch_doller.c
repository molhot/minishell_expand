/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_swaitch.doller.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 00:29:13 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/11 00:29:13 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	switch_doller(char **new_word, char **args)
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
