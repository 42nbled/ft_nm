NAME = ft_nm

OBJ_DIR = objs

SRCS =	srcs/main.c \
		srcs/lst.c \
		srcs/parse_table_32b.c \
		srcs/parse_table_64b.c \
		srcs/sort.c \
		srcs/test_function.c

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iheader # -g -ggdb #-32m

$(NAME): $(OBJS)
	@echo "\033[0;34mCompiling $(NAME)...\033[0m"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[0;32mCompilation done.\033[0m"

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\033[0;34mCleaning objects...\033[0m"
	rm -rf $(OBJ_DIR)
	@echo "\033[0;32mCleaning done.\033[0m"

fclean: clean
	@echo "\033[0;34mCleaning executable...\033[0m"
	rm -f $(NAME) result_ft_nm.txt result_nm.txt a b
	@echo "\033[0;32mCleaning done.\033[0m"

re: fclean all

-include $(OBJS:.o=.d)
.PHONY: all clean fclean re
