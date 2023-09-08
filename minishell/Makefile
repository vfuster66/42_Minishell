# Compiler options
CC = gcc
CFLAGS = -Werror -Wall -Wextra -gdwarf-4 -g3

# Output executable name
NAME = minishell

# Directories
SRC_PATH = sources
OBJ_PATH = objects

# Source files
SRC = main.c ft_strcmp.c ft_calloc.c ft_isdigit.c \
      ft_isspace.c ft_strdup.c ft_strjoin.c ft_strtrim.c \
      ft_isalnum.c ft_isalpha.c ft_itoa.c ft_memset.c \
      ft_substr.c ft_split.c ft_strchr.c ft_putchar_fd.c \
      ft_putendl_fd.c ft_putstr_fd.c ft_strlcpy.c ft_strlen.c \
      ft_strncmp.c

# Object files
OBJ = $(SRC:.c=.o)
OBJS = $(addprefix $(OBJ_PATH)/, $(OBJ))

# Include directories
INC = -I includes

# Libraries
LIBS = -lreadline

# Targets
all: $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)
	@echo "Minishell compiled"

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)
	@echo "🧽 🪣 🧹 Minishell cleaned"

re: fclean all

.PHONY: all re clean fclean



