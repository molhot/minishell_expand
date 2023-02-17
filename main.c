#include "minishell.h"

t_map	*g_env;

char  *get_name(char *name_and_value)
{
	size_t	len;
	size_t	j;
	char	*name;

	len = 0;
	while(name_and_value[len] != '\0')
	{
		if (name_and_value[len] == '=')
			break;
		len++;
	}
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		fatal_error("malloc");
	j = 0;
	while (len)
	{
		name[j] = name_and_value[j];
		j++;
		len--;
	}
	name[j] = '\0';
	return (name);
}

void	env_init(t_map **env, char **envp)
{
	size_t	i;
	char	*name;
	char	*value;

	i = 0;
	*env = malloc(sizeof(t_map));
	while (envp[i] != NULL)
	{
		name = get_name(envp[i]);
		value = getenv(name);
		map_set(env, name, value);
		i++;
	}
}

bool	is_builtin(char *line)
{
	if (strncmp("export", line, ft_strlen("export")) == 0)
		return (true);
	else if (strncmp("env", line, ft_strlen("env")) == 0)
		return (true);
	else if (strncmp("pwd", line, ft_strlen("pwd")) == 0)
		return (true);
	else if (strncmp("cd", line, ft_strlen("cd")) == 0)
		return (true);
	else if (strncmp("exit", line, ft_strlen("exit")) == 0)
		return (true);
	else if (strncmp("unset", line, ft_strlen("unset")) == 0)
		return (true);
	else if (strncmp("echo", line, ft_strlen("echo")) == 0)
		return (true);
	else
    	return (false);
}

int main()
{
	char		*line;
	t_token		*tok;
	t_node		*node;
	extern char **environ;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);

	rl_outstream = stderr;
	env_init(&g_env, environ);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line != 0)
		{
			if (*line)
				add_history(line);
			if (line[0] == '/' || line[0] == '.')
				abusolute_path(line);
			else
			{
				tok = tokenizer(line);
				node = parse(tok);
				expand(node);
<<<<<<< HEAD
				//if (node->next == NULL && is_builtin(node->command->args->word))
				//	do_builtin(node->command->args->word, node->command);
				//else
				exec(node);
||||||| c42bd58
				if (node->next == NULL && is_builtin(node->command->args->word))
					do_builtin(node->command->args->word, node->command);
				else
					exec(node);
=======
				// if (node->next == NULL && is_builtin(node->command->args->word))
				// 	do_builtin(node->command->args->word, node->command);
				exec(node);
>>>>>>> refs/remotes/origin/master
				if (tok != NULL)
					free_token(tok);
			}
		}
		free(line);
	}
	exit(0);
}
