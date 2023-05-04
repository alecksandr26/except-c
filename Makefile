# @file Makefile
# @brief The compilation script
# @author Erick Carrillo.
# @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All right reserved.
# @license This project is released under the MIT License 

# For debuggin
.SUFFIXES:
%:: SCCS/s.%
%:: RCS/%
%:: RCS/%,v
%:: %,v
%:: s.%

define \n


endef

C = cc
C_DEBUG_FLAGS = -ggdb -pedantic -Wall
C_COMPILE_FLAGS = -O2 -DNDEBUG -fno-stack-protector -z execstack -no-pie
C_FLAGS = $(C_DEBUG_FLAGS)
N = nasm
N_DEBUG_FLAGS = -g -f elf64
N_COMPILE_FLAGS = -f elf64
N_FLAGS = $(N_DEBUG_FLAGS)

AR = ar rc
CF = clang-format -i

V = valgrind
V_FLAGS = --leak-check=full --track-origins=yes -s  --show-leak-kinds=all

OBJ_DIR = obj
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
LIB_DIR = lib

INSTALL_LIB_DIR = /usr/lib
INSTALL_INCLUDE_DIR = /usr/include

# The source code of tests
TEST_SRC_DIR = $(addprefix $(TEST_DIR)/, src)
TEST_BIN_DIR = $(addprefix $(TEST_DIR)/, bin)

# The dependencies
OBJS = $(addprefix $(OBJ_DIR)/, except.o stackjmp.o)

# The complete library
LIB = $(addprefix $(LIB_DIR)/, libexcept.a)

INTERFACES = $(addprefix $(INCLUDE_DIR)/, except.h stackjmp.h)

# The tests
TESTS = $(addprefix $(TEST_BIN_DIR)/, 	test_stackjmp.out test_except.out)


# Compile everything
.PHONY: all clean compile install format format_$(SRC_DIR)/%.c format_$(INCLUDE_DIR)/%.h \
	format_$(TEST_SRC_DIR)/%.c

all: $(OBJ_DIR) $(LIB_DIR) $(TEST_BIN_DIR) $(OBJS) $(LIB) $(TESTS)

$(OBJ_DIR):
	@echo Creating: $@
	@mkdir -p $@

$(LIB_DIR):
	@echo Creating: $@
	@mkdir -p $@

# Create the output binary
$(TEST_BIN_DIR):
	@echo Creating: $@
	@mkdir -p $@

# Compile all the dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INTERFACES)
	@echo Compiling: $< -o $@
	@$(C) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm $(INTERFACES)
	@echo Compiling: $< -o $@
	@$(N) $(N_FLAGS) $< -o $@

# Archive the whole dependecies
$(LIB): $(OBJS) $(INTERFACES)
	@echo Archiving: $(OBJS) -o $@
	@$(AR) $@ $(OBJS)
	@ranlib $@

$(TEST_BIN_DIR)/test_%.out: $(TEST_SRC_DIR)/test_%.c $(LIB) $(INTERFACES)
	@echo Compiling: $(word 1, $^) $(word 2, $^) -o $@
	@$(C) $(C_FLAGS) $(word 1, $^) $(word 2, $^) -o $@

# To run an specifyc test
test_%.out: $(TEST_BIN_DIR)/test_%.out
	@echo $@:
	@$(V) $(V_FLAGS) ./$<
	@echo Passed:

# To Run all the tests
tests: $(notdir $(TESTS))

# To clean any compiled object
clean_$(OBJ_DIR)/%.o:
	@echo Removing: $(patsubst clean_%, %, $@)
	@rm $(patsubst clean_%, %, $@)

# To clean any archive library
clean_$(LIB_DIR)/%.a:
	@echo Removing: $(patsubst clean_%, %, $@)
	@rm $(patsubst clean_%, %, $@)

# To clean any compiled test
clean_$(TEST_BIN_DIR)/%.out:
	@echo Removing: $(patsubst clean_%, %, $@)
	@rm $(patsubst clean_%, %, $@)

# Remove all the compiled dependencies and tes
clean: 	$(addprefix clean_, 	$(wildcard $(OBJ_DIR)/*.o) \
				$(wildcard $(LIB_DIR)/*.a) \
				$(wildcard $(TEST_BIN_DIR)/*.out))
ifneq ("$(wildcard $(OBJ_DIR))", "")
	@echo Removing: $(OBJ_DIR)
	@rmdir $(OBJ_DIR)
endif

ifneq ("$(wildcard $(TESTS_BIN_DIR))", "")
	@echo Removing: $(TEST_BIN_DIR)
	@rmdir $(TESTS_BIN_DIR)
endif

ifneq ("$(wildcard $(LIB_DIR))", "")
	@echo Removing: $(LIB_DIR)
	@rmdir $(LIB_DIR)
endif

# Clean objects and libs and recompile with optimizations
compile: C_FLAGS = $(C_COMPILE_FLAGS)
compile: N_FLAGS = $(N_COMPILE_FLAGS)
compile: $(OBJ_DIR) $(LIB_DIR) $(TEST_BIN_DIR) \
	$(addprefix clean_, 	$(wildcard $(OBJ_DIR)/*.o) \
				$(wildcard $(LIB_DIR)/*.a) \
				$(wildcard $(TEST_BIN_DIR)/*.out)) \
	$(LIB)

# Install header files
$(INSTALL_INCLUDE_DIR)/%.h: $(INCLUDE_DIR)/%.h
	@echo Installing: $< -o $@
	sudo install $< $@

# Install the library
install: compile $(addprefix $(INSTALL_INCLUDE_DIR)/, $(notdir $(INTERFACES)))
	@echo Installing: $(LIB) -o $(INSTALL_LIB_DIR)/$(notdir $(LIB))
	sudo install $(LIB) $(INSTALL_LIB_DIR)/$(notdir $(LIB))
	@echo Installed:

format_$(SRC_DIR)/%.c:
	@echo Formatting: $(patsubst format_%, %, $@)
	@$(CF) $(patsubst format_%, %, $@)

format_$(INCLUDE_DIR)/%.h:
	@echo Formatting: $(patsubst format_%, %, $@)
	@$(CF) $(patsubst format_%, %, $@)

format_$(TEST_SRC_DIR)/%.c:
	@echo Formatting: $(patsubst format_%, %, $@)
	@$(CF) $(patsubst format_%, %, $@)

# To format all the code
format: $(addprefix format_, 	$(wildcard $(SRC_DIR)/*.c) \
				$(wildcard $(INCLUDE_DIR)/*.h) \
				$(wildcard $(TEST_SRC_DIR)/*.c))
