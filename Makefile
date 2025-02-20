NAME = ft_nm

SRC_DIR = srcs
OBJ_DIR = objs
INC_DIR = header

SRCS =	main.c \
		arg_check.c \
		lst.c \
		parse_table_32b.c \
		parse_table_64b.c \
		get_symbol_type.c \
		print_nm.c \
		sort.c \
		test_function.c

SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) # -g -ggdb #-32m

$(NAME): $(OBJS)
	@echo "\033[0;34mCompiling $(NAME)...\033[0m"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[0;32mCompilation done.\033[0m"

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
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
