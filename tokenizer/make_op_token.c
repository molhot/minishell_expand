/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_op_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 22:38:37 by user              #+#    #+#             */
/*   Updated: 2023/02/16 22:39:10 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *operator(char **rest, char *line)
{
	static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t i = 0;
	char *op;

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (op == NULL)
				fatal_error("strdup");
			*rest = line + strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	fatal_error("Unexpected operator");
}