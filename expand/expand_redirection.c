/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 08:22:33 by marvin            #+#    #+#             */
/*   Updated: 2023/02/22 08:22:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quote_append(char **args, char **new_word)
{
	char	*tmp;

	(*args)++;
	tmp = *args;
	if (*(tmp - 1) == '\'')
		append_single(&(*args), &(*new_word));
	else if (*(tmp - 1) == '\"')
		append_double(&(*args), &(*new_word));
}

char	*expand_args_redirect(char *args, char *args_free)
{
	char	*new_word;

	new_word = NULL;
	while (*args != '\0')
	{
		if (*args == '\\')
		{
			args++;
			append_char(&new_word, *args++);
		}
		if ((*args == '\'' || *args == '\"') && *(args + 1) != '\0')
			quote_append(&args, &new_word);
		else if (*args == '$' && *(args + 1) == '?')
			expand_dolleeques(&new_word, &args, args);
		else if (*args == '$')
			expand_doller(&new_word, &args, args);
		else
			append_char(&new_word, *args++);
	}
	free(args_free);
	return (new_word);
}

void	check_doller(char **rest, char *p, t_redirect *redirect)
{
	char	*name;
	char	*value;

	name = ft_calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	p++;
	if (!ft_isalpha(*p) && *p != '_')
	{
		*rest = p + 1;
		free(name);
		return ;
	}
	append_char(&name,*p++);
	while (ft_isalpha(*p) != 0 || *p == '_' || ft_isdigit(*p) != 0)
		append_char(&name,*p++);
	value = map_get(g_env, name);
	free(name);
	if (value == NULL || ft_strchr(value, ' ') != NULL)
		redirect->ambigous = true;
	else
		redirect->ambigous = false;
	*rest = p;
}

void	specialparam_check(t_redirect *redirect)
{
	char	*args;
	char	type;

	while (redirect != NULL)
	{
		args = redirect->file_path;
		while (*args != '\0')
		{
			if (*args == '\\')
				args = args + 2;
			else if (*args == '\'' || *args == '\"')
			{
				type = *args;
				args++;
				while (*args != type)
					args++;
				args++;
			}
			else if (*args == '$' && *(args + 1) == '?')
				args = args + 2;
			else if (*args == '$' && ((*(args + 1) == '\'') || (*(args + 1) == '\"')))
				args++;
			else if (*args == '$')
				check_doller(&args, args, redirect);
			else
				args++;
		}
		redirect = redirect->next;
	}
}

t_redirect	*expand_redirect_ten(t_redirect *redirect)
{
	t_redirect	*f_redirect;

	f_redirect = redirect;
	specialparam_check(redirect);
	while (redirect != NULL)
	{
		if (redirect->ambigous == false)
			redirect->file_path = \
			expand_args_redirect(redirect->file_path, redirect->file_path);
		redirect = redirect->next;
	}
	return (f_redirect);
}
