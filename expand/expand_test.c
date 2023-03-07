/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 20:59:01 by marvin            #+#    #+#             */
/*   Updated: 2023/02/26 20:59:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_map	*g_env;

int	main(void)
{
	char	*line;

	line = "\"'\"hoge\"'\" > \"\"'hoge'\"\"";
	t_token *t = tokenizer(line);
	t_node *n = parse(t);
	expand(n);
	printf("%s\n", n->command->args->word);
	printf("%s\n", (*n->command->redirect)->file_path);
}
