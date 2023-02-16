#include "../minishell.h"

t_map	*g_env;


/*void	shell_works(char *line)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenizer(line);
	expand(tok);
	node = parse(tok);
	exec(&node);
	if (tok != NULL)
		free_token(tok);
	if (node != NULL)
		free_command_node(node);
}*/

void  sigint_handler()
{
  write(1, "\n", 1);
  rl_on_new_line();
  //rl_replace_line("", 0); 
  rl_redisplay(); 
}

#include <string.h>
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
	// error
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
	// error
	while (envp[i] != NULL)
	{
		name = get_name(envp[i]);
		value = getenv(name);
		map_set(env, name, value);
		i++;
	}
}

bool  is_builtin(char *line)
{
  char		*set[] = {"export", "env", "pwd", "cd", "exit", "unset", "echo", NULL};
  size_t	i;

  i = 0;
  while (set[i])
  {
	if (strncmp(set[i], line, ft_strlen(set[i])) == 0)
	{
		//puts("found!");
		return true;
	}
    i++;
  }
    return false;
}

void	do_builtin(char *line, t_command *command)
{
	if (strcmp(line, "env") == 0)
		ms_env();
	else if (strncmp(line, "export", 6) == 0)
		ms_export(line, command);
	else if (strcmp(line, "pwd") == 0)
		ms_pwd();
	else if (strncmp(line, "cd", 2) == 0)
		ms_cd(line, command);
	else if (strncmp(line, "exit", 4) == 0)
		ms_exit(line, command);
	else if (strncmp(line, "unset", 5) == 0)
		ms_unset(line, command);
	else if (strncmp(line, "echo", 4) == 0)
		ms_echo(line, command);
}

int main()
{
	char	  *line;
	t_token	*tok;
  	t_node  *node;
	//t_node	*fnode;
	extern char **environ;

	rl_outstream = stderr;
	env_init(&g_env, environ);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line)
			add_history(line);
		if (line[0] == '/' || line[0] == '.')
			abusolute_path(line);
		// else if (is_builtin(line))
		// 	do_builtin(line);
		else
		{
			tok = tokenizer(line);
			node = parse(tok);
			expand(node);
			if (is_builtin(node->command->args->word) && node->next == NULL)
				do_builtin(node->command->args->word, node->command);
			else
			{
				exec(node);
				if (tok != NULL)
					free_token(tok);
			}
		}
		free(line);
	}
	exit(0);
}