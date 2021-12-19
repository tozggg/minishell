NAME	= a.out
SRCS	= main.c \
		  exec_line.c \
		  exec_command.c \
		  parse/cmd.c \
		  parse/env_key.c \
		  parse/env.c \
		  parse/separate.c \
		  parse/parse_env.c \
		  parse/parse.c \
		  parse/parse_utils.c \
		  redirection.c \
		  redirection_validate.c \
		  redirection_heredoc.c \
		  ft_execvpe.c \
		  builtin/builtin.c \
		  builtin/echo.c \
		  builtin/export.c \
		  builtin/unset.c \
		  builtin/env.c \
		  builtin/exit.c \
		  error.c \
		  utils.c
HEADERS = minishell.h
OBJS 	= $(SRCS:.c=.o)
CFLAGS	= -Wall -Wextra
LDLIBS	= -lreadline -Llibft -lft

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

.PHONY: all clean fclean re test cleanlib

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) bonus -C libft
	gcc $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

cleanlib:
	$(MAKE) -C libft fclean

fclean: cleanlib clean
	rm -rf $(NAME)

re: fclean all

test: CFLAGS += -g3 -fsanitize=address -D DEBUG
test: clean all
