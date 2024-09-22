NAME = minishell
CC = gcc
CFLAGS = -Werror -Wextra -Wall -I.
RM = rm -rf

SRC = 	./parsing/tokenization.c \
		./parsing/parsing_quotes.c \
		./parsing/token_special.c \
		./parsing/token_special_1.c \
		./parsing/parse.c \
		./parsing/syntax_controll.c \
		./utils/single_quotes.c \
		./utils/util_1.c \
		./utils/util_2.c \
		./utils/util_3.c \
		./utils/util_4.c \
		./utils/double_quotes.c \
		./init.c \
		./ft_exec.c \
		./main.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C ./libft
	@$(CC) $(CFLAGS) $(OBJS) -lreadline ./libft/libft.a -o $(NAME)

clean:
	@$(MAKE) -C ./libft clean
	@$(RM) $(OBJS)

fclean:
	@$(MAKE) -C ./libft fclean
	@$(RM) $(OBJS) $(NAME)

re: fclean all

.PHONY: all re clean fclean

valgrind: re
	@valgrind -s --leak-check=full --show-leak-kinds=all --log-file=output.log ./minishell