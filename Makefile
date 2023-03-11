NAME	=	minishell
SRCS	=	tokenizer/tokenizer.c tokenizer/create_token.c\
			tokenizer/make_op_token.c tokenizer/make_redirect_token.c tokenizer/make_wd_token.c\
			syntax_check/syntax_check.c syntax_check/tokencheck.c syntax_check/tokencheck_quote.c\
			parser/parser.c parser/parse_word.c parser/parse_redirect.c parser/parser_utils.c\
			expand/expand.c expand/expand_doller.c expand/expand_appendchar.c expand/expand_specialchar.c expand/export_expand.c\
			expand/expand_simplecommand.c expand/expand_redirection.c expand/simplecommand_utils.c expand/quote_manage.c expand/doller_manage.c\
			exec_func/exec_func.c exec_func/exec_builtin.c exec_func/pipe_func.c exec_func/redirect_file.c expand/expand_swaitch_doller.c\
			exec_func/exec_absolutepath.c exec_func/exec_utils.c exec_func/stashfd_helper.c exec_func/builtin_exec.c\
			word_rolechecker/is_blank.c word_rolechecker/is_metacharacter.c word_rolechecker/is_space.c word_rolechecker/consume_blank.c\
			word_rolechecker/is_operator.c word_rolechecker/is_redirect.c word_rolechecker/is_word.c word_rolechecker/startswith_keyword.c\
			builtin_command/builtin_cd.c builtin_command/builtin_echo.c builtin_command/builtin_env.c\
			builtin_command/builtin_exit.c builtin_command/builtin_export.c builtin_command/builtin_pwd.c\
			builtin_command/builtin_unset.c builtin_command/command_to_array.c builtin_command/builtin_check.c\
			builtin_command/puts_errorstring.c builtin_command/builtin_export_no.c\
			signalhandler/sigint_handler.c\
			map/map.c map/map_utils.c\
			free/free_token.c\
			env_ready.c main.c error.c ms_atoi.c
OBJS	=	$(SRCS:%.c=%.o)
LIBSRCS	=	libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isascii.c libft/ft_isalnum.c libft/ft_isprint.c\
			libft/ft_strlen.c libft/ft_memset.c libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_strlcat.c\
			libft/ft_strlcpy.c libft/ft_toupper.c libft/ft_tolower.c libft/ft_strnstr.c libft/ft_strncmp.c\
			libft/ft_strchr.c libft/ft_strrchr.c libft/ft_atoi.c libft/ft_memcmp.c libft/ft_memchr.c \
			libft/ft_calloc.c libft/ft_strdup.c libft/ft_strjoin.c libft/ft_split.c\
			libft/ft_itoa.c libft/ft_substr.c libft/ft_strtrim.c\
			libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c\
			libft/ft_strmapi.c libft/ft_striteri.c libft/ft_strcmp.c\
			libft/ft_lstnew.c libft/ft_lstadd_front.c libft/ft_lstsize.c libft/ft_lstlast.c libft/ft_lstadd_back.c\
			libft/ft_lstclear.c libft/ft_lstiter.c libft/ft_lstmap.c libft/ft_strndup.c
LIBOBJ = $(LIBSRCS:%.c=%.o)
CC  	=	gcc -g
CFLAGS	=	-Wall -Werror -Wextra
RLDIR	=	$(shell brew --prefix readline)
SANI	= -fsanitize=address

all:$(NAME)

$(NAME):$(OBJS) $(LIBOBJ)
	- make -C libft
	$(CC) $(CFLAGS) $(OBJS) $(SANI) -Llibft -L$(RLDIR)/lib -I$(RLDIR)/include -o $(NAME) -lreadline -lft

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -I$(RLDIR)/include -c  $< -o $@ 

debug:$(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	- make fclean -C libft
	rm -f $(NAME)

re: fclean all
