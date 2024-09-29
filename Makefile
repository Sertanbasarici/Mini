NAME = minishell
CC = gcc
CFLAGS =  -Werror -Wextra -Wall -I.
RM = rm -rf

READLINE_DIR = ./readline-8.2
READLINE_V = $(READLINE_DIR)/lib/libreadline.a
INCLUDES = includes -I${READLINE_DIR}/include
LIBS = -ltermcap -lncurses -L$(READLINE_DIR)/lib/ -lreadline

SRC = 	./parsing/tokenization.c \
		./parsing/parsing_quotes.c \
		./parsing/token_special.c \
		./parsing/token_special_1.c \
		./parsing/parse.c \
		./parsing/syntax_controll.c \
		./parsing/ft_expand_envp.c \
		./parsing/parse_2.c \
		./utils/single_quotes.c \
		./utils/util_1.c \
		./utils/util_2.c \
		./utils/util_3.c \
		./utils/util_4.c \
		./utils/util_5.c \
		./utils/util_6.c \
		./utils/util_7.c \
		./utils/double_quotes.c \
		./utils/ft_minishell_init.c \
		./utils/ft_exit_bits.c \
		./utils/ft_signals.c \
		./utils/free_all.c \
		./utils/free_all_2.c \
		./commands/ft_cd.c \
		./commands/ft_echo.c \
		./commands/ft_env.c \
		./commands/ft_exit.c \
		./commands/ft_export.c \
		./commands/ft_pwd.c \
		./commands/ft_unset.c \
		./execution/ft_execute.c \
		./execution/ft_builtins.c \
		./execution/ft_heredoc.c \
		./execution/ft_heredoc_utils.c \
		./execution/ft_pipe.c \
		./execution/ft_execve.c \
		./execution/ft_redirection.c \
		./init.c \
		./main.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C ./libft
	@$(CC) $(CFLAGS) $(OBJS) ./libft/libft.a -o $(NAME) -I $(INCLUDES) $(LIBS)

%.o: %.c $(READLINE_V)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDES)

clean:
	@$(MAKE) -C ./libft clean
	@$(RM) $(OBJS)

fclean:
	@$(MAKE) -C ./libft fclean
	@$(RM) $(OBJS) $(NAME)

re: fclean all

.PHONY: all re clean fclean

$(READLINE_DIR):
	@echo "Downloading Readline-8.2..."
	@curl https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz -o readline-8.2.tar.gz 2>&1 | awk '{printf "."; fflush()}'
	@echo ""
	@mkdir ./readline-8.2
	@tar xvfz readline-8.2.tar.gz > /dev/null 2> /dev/null
	@rm readline-8.2.tar.gz

$(READLINE_V): | $(READLINE_DIR)
	@echo "Compiling Readline-8.2..."
	@cd readline-8.2 && ./configure --prefix=$(PWD)/$(READLINE_DIR) 2>&1 | awk '{printf "."; fflush()}'
	@cd readline-8.2 && make install 2>&1 | awk '{printf "."; fflush()}'
	@printf "\n"
	#@rm -rf readline-8.2