NAME = a.out
SRCS = exec_line.c main.c utils.c exec_command.c redirection.c \
	   redirection_validate.c \
	   redirection_heredoc.c \
	   ft_execvpe.c \
	   error.c \
	   builtin/builtin.c \
	   builtin/echo.c \
	   builtin/exit.c \
	   builtin/env.c \
	   builtin/export.c \
	   builtin/unset.c \
	   parse/tmp_listfunc.c \
	   parse/tmp_parser.c

OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra
LDLIBS = -lreadline -Llibft -lft

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		CFLAGS	+= -I${HOME}/.brew/opt/readline/include
		LDLIBS	+= -L${HOME}/.brew/opt/readline/lib
	endif
	ifeq ($(UNAME_M),arm64)
		CFLAGS	+= -I/opt/homebrew/opt/readline/include
		LDLIBS	+= -L/opt/homebrew/opt/readline/lib
	endif
endif
.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) bonus -C libft
	gcc $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

%.o: %.c minishell.h
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

test: CFLAGS += -g3 -fsanitize=address -D DEBUG
test: clean all
