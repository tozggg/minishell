NAME = a.out
SRCS = exec_line.c main.c utils.c exec_command.c redirection.c \
	   ft_execvpe.c \
	   builtin/builtin.c \
	   parse/tmp_listfunc.c \
	   parse/tmp_parser.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) bonus -C libft
	gcc $(CFLAGS) $(OBJS) -lreadline -Llibft -lft -o $(NAME)

%.o: %.c minishell.h
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

test: CFLAGS += -g3 -fsanitize=address -D DEBUG
test: clean all
