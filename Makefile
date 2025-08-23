NAME = ft_strace 
MODE ?= release

OBJ_DIR = obj-$(MODE)
INCLUDES = -Iincludes

CXX = cc
CXXFLAGS = -Wall -Werror -Wextra -MD $(INCLUDES)

ifeq ($(MODE), debug)
	CXX = gcc
	CXXFLAGS = -Wall -Wextra -MD $(INCLUDES) -g3
endif

VPATH = srcs

SRCS =	main.c			\
		
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)
BIN = $(NAME)

RESET			= \033[0m
GRAY			= \033[90m
RED 			= \033[31m
GREEN 			= \033[32m
YELLOW 			= \033[33m
BLUE 			= \033[34m

all:
	$(MAKE) $(BIN)
	printf "$(RESET)"

debug:
	$(MAKE) MODE=debug all

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c Makefile |  $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	printf "$(GRAY)compiling: $(BLUE)%-40s $(GRAY)[%d/%d]\n" "$<" "$$(ls $(OBJ_DIR) | grep -c '\.o')" "$(words $(SRCS))"

clean:
	rm -rf obj-*

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug

-include $(DEPS)
.SILENT:
MAKEFLAGS=--no-print-directory
