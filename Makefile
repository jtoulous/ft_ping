NAME = ft_ping
SRC = ft_ping.c utils.c parsing.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJ)
	@clang $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

PHONY: all clean fclean re