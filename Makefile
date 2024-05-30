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
C_DEBUG_FLAGS = -Wall -Wextra -pedantic -ggdb
C_COMPILE_FLAGS = -O2 -DNDEBUG -fno-stack-protector -z execstack -no-pie -fPIC
C_FLAGS = $(C_DEBUG_FLAGS)
N = nasm
N_DEBUG_FLAGS = -g -f elf64
N_COMPILE_FLAGS = -f elf64
N_FLAGS = $(N_DEBUG_FLAGS)

AR = ar rc
CF = clang-format -i

V = valgrind
V_FLAGS = --leak-check=full --track-origins=yes -s  --show-leak-kinds=all --max-stackframe=8016174317795404487

OBJ_DIR = obj
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
EXAMPLE_DIR = examples
LIB_DIR = lib

BUILD_DIR = build
UPLOAD_DIR = upload
PKGNAME = except-c
GCU = ssh://aur@aur.archlinux.org/$(PKGNAME).git # git clone

# For installation
M = makepkg
M_FLAGS = -f --config .makepkg.conf --skipinteg --noextract

TEST_SRC_DIR = $(addprefix $(TEST_DIR)/, src)
TEST_BIN_DIR = $(addprefix $(TEST_DIR)/, bin)

OBJS = $(addprefix $(OBJ_DIR)/, except.o assert.o)

LIB = $(addprefix $(LIB_DIR)/, libexcept.a)

TESTS = $(addprefix $(TEST_BIN_DIR)/, 	test_except.out test_simple.out test_finally.out test_else.out \
					test_assert.out)

EXAMPLES = $(addprefix $(EXAMPLE_DIR)/, example_general_purpuse.c)

# Compile everything
.PHONY: all clean compile install format
all: $(OBJ_DIR) $(LIB_DIR) $(TEST_BIN_DIR) $(OBJS) $(LIB) $(TESTS)

$(OBJ_DIR):
	@echo Creating: $@
	@mkdir -p $@

$(LIB_DIR):
	@echo Creating: $@
	@mkdir -p $@

$(UPLOAD_DIR):
	@echo Creating: $@
	@mkdir -p $@

# Create the output binary
$(TEST_BIN_DIR):
	@echo Creating: $@
	@mkdir -p $@

# Compile all the dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo Compiling: $< -o $@
	@$(C) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | $(OBJ_DIR)
	@echo Compiling: $< -o $@
	@$(N) $(N_FLAGS) $< -o $@

# Archive the whole dependecies
$(LIB): $(OBJS) | $(LIB_DIR)
	@echo Archiving: $(OBJS) -o $@
	@$(AR) $@ $(OBJS)
	@ranlib $@

$(TEST_BIN_DIR)/test_%.out: $(TEST_SRC_DIR)/test_%.c $(LIB) | $(TESTS_BIN_DIR)
	@echo Compiling: $^ -o $@
	@$(C) $(C_FLAGS) $^ -o $@

# To run an specifyc test
test_%.out: $(TEST_BIN_DIR)/test_%.out
	@echo $@
	@ DEBUGINFOD_URLS="https://debuginfod.archlinux.org" $(V) $(V_FLAGS) ./$<
	@echo Passed:

# To Run all the tests
test: $(notdir $(TESTS))

$(EXAMPLE_DIR)/%.out: $(EXAMPLE_DIR)/%.c $(LIB)
	@echo Compiling: $^ -o $@
	@$(C) $(C_FLAGS) $^ -o $@

example_%.out: $(EXAMPLE_DIR)/example_%.out
	@echo $@
	@$(V) $(V_FLAGS) ./$<


example: $(notdir $(EXAMPLES))


# Remove all the compiled dependencies and tes
clean:
ifneq ("$(wildcard $(OBJ_DIR))", "")
	@rm -vr $(OBJ_DIR)
endif

ifneq ("$(wildcard $(TESTS_BIN_DIR))", "")
	@rm -vr $(TESTS_BIN_DIR)
endif

ifneq ("$(wildcard $(LIB_DIR))", "")
	@rm -vr $(LIB_DIR)
endif

# Clean objects and libs and recompile with optimizations
compile: C_FLAGS = $(C_COMPILE_FLAGS)
compile: N_FLAGS = $(N_COMPILE_FLAGS)
compile: clean $(OBJ_DIR) $(OBJS) $(LIB_DIR) $(LIB)

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

pkg:
	$(M) $(M_FLAGS)	

$(UPLOAD_DIR)/$(PKGNAME): $(UPLOAD_DIR)
	@cd $< && git clone $(GCU)

upload-aur: $(UPLOAD_DIR)/$(PKGNAME)
	@cp PKGBUILD $</
	@cd $</ && $(M) --printsrcinfo > .SRCINFO
	@cd $</ && git add PKGBUILD .SRCINFO
	@echo -n "Commit-msg: "
	@read commitmsg
	@cd $</ && git commit -m commitmsg
	@cd $</ && git push
