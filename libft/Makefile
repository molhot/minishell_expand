LIBSRCS			=	ft_isalpha.c ft_isdigit.c ft_isascii.c ft_isalnum.c ft_isprint.c\
					ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcat.c\
					ft_strlcpy.c ft_toupper.c ft_tolower.c ft_strnstr.c ft_strncmp.c\
					ft_strchr.c ft_strrchr.c ft_atoi.c ft_memcmp.c ft_memchr.c \
					ft_calloc.c ft_strdup.c ft_strjoin.c ft_split.c\
					ft_itoa.c ft_substr.c ft_strtrim.c\
					ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
					ft_strmapi.c ft_striteri.c ft_strcmp.c\
					ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c\
					ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_strndup.c

LIBOBJS		=	$(LIBSRCS:.c=.o)

CC			=	cc
RM			=	rm -f
CFLAGS		=	-Wall -Wextra -Werror

NAME		=	libft.a

all:		$(NAME)

$(NAME):	$(LIBOBJS)
	ar rcs $(NAME) $(LIBOBJS)

clean:		
	$(RM) $(LIBOBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean $(NAME)

test:	all
	gcc test.c $(NAME) -o test;
	./test;

.PHONY:		all clean fclean re
