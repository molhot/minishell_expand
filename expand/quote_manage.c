/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 09:43:35 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/10 09:43:35 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quote_append_indoller(char type, char **new, char **args)
{
	append_char(&(*new), **args);
	(*args)++;
	if (**args == '\0')
		return ;
	while (**args != type)
	{
		if (**args == '\\')
		{
			append_char(&(*new), **args);
			(*args)++;
		}
		append_char(&(*new), **args);
		(*args)++;
	}
	append_char(&(*new), **args);
	(*args)++;
}
