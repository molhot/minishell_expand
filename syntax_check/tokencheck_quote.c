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

bool	wd_check_inquote(char **str, char *tmp_str)
{
	char	type;

	while (**str != '\'' && **str != '\"')
		(*str)++;
	type = **str;
	(*str)++;
	while (**str != type && **str != '\0')
		(*str)++;
	if (**str == '\0')
		return (show_error(tmp_str, ft_strlen(tmp_str)));
	return (true);
}
