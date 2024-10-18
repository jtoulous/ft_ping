NAME = ft_ping
SRC = ft_ping.c utils/parsing.c utils/logs.c utils/tools.c utils/errors.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJ)
	@clang $(OBJ) -o $(NAME) -lresolv -lm

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

PHONY: all clean fclean re