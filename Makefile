CC = g++
CPP_VERSION = -std=c++11
CFLAGS = -Wall -Wextra -Werror $(CPP_VERSION) -g3

# All

all: bin test

clean: bin_clean test_clean

fclean: bin_fclean test_fclean

re: fclean all


# Program

NAME =	computor
MAIN =	main.cpp
FILE =	complex.cpp \
		math.cpp \
		token.cpp \
		lexer.cpp \
		ast.cpp \
		parser.cpp \
		interpreter.cpp \
		output.cpp

SRC_FILE = $(MAIN) $(FILE)
SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/,$(SRC_FILE))

INC = -I include

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC_FILE:%.cpp=%.o))

bin: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

bin_clean:
	rm -rf $(OBJ_DIR)

bin_fclean: bin_clean
	rm -f $(NAME)

bin_re: bin_fclean $(NAME)


# Test

TEST_NAME = computor_test

TEST_FILE = test_main.cpp \
			test_lexer.cpp \
			test_ast.cpp

TEST_DIR = test
TEST = $(addprefix $(TEST_DIR)/,$(TEST_FILE))

TEST_OBJ_DIR = obj_test
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/,$(TEST_FILE:%.cpp=%.o))

DEP = $(addprefix $(OBJ_DIR)/,$(FILE:%.cpp=%.o))

test: $(TEST_NAME)

$(TEST_NAME):  $(OBJ_DIR) $(DEP) $(TEST_OBJ_DIR) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(TEST_NAME) $(DEP) $(TEST_OBJ)

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

test_clean:
	rm -rf $(DEP) $(TEST_OBJ_DIR)

test_fclean: test_clean
	rm -f $(TEST_NAME)

test_re: test_fclean $(TEST_NAME)


.PHONY: all clean re fclean bin_clean bin_fclean test_clean test_fclean