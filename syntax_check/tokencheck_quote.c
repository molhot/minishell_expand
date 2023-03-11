/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokencheck_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochitteiunon? <sakata19991214@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 23:50:00 by mochitteiun       #+#    #+#             */
/*   Updated: 2023/03/10 23:50:00 by mochitteiun      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	after_backslush(char **str)
{
	if (**str == '\\')
		(*str)++;
	(*str)++;
}

bool	wd_check_inquote(char **str, char *tmp_str)
{
	char	type;

	while (**str != '\0')
	{
		while (**str != '\'' && **str != '\"')
		{
			if (**str == '\0')
				return (true);
			after_backslush(&(*str));
		}
		type = **str;
		(*str)++;
		while (**str != type && **str != '\0')
			after_backslush(&(*str));
		if (**str == '\0')
			return (show_error(tmp_str, ft_strlen(tmp_str)));
		(*str)++;
	}
	return (true);
}
