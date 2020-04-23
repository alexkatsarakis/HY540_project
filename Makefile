# Compiler
CC=g++

# Current directory
PROJECTDIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# Project structure
ODIR=obj
SRCDIR=src
INCLDIR=include
TESTDIR=tests

# Name of the executable file
EXECUTABLE=interpreter.out

# Name of a simple test file that can be used as input
SAMPLE_TEST=$(TESTDIR)/test.js

# Specify the include directories
# Find all source files
LINKER_DIRS:=$(shell cd $(INCLDIR); ls )
LINKER_PREFIX = -I$(PROJECTDIR)${INCLDIR}
LINKER= $(patsubst %,$(LINKER_PREFIX)/%,$(LINKER_DIRS))

# Set compilation flags
# -Wshadow: Warn whenever a local variable or type declaration shadows another
#           variable, parameter, type or when a built-in function is shadowed
# -Wold-style-cast: Warn oc C-style cast
# -Wpedantic:
# -Wfloat-equal: Warn if floating point values are used in equality comparisons
# -Wpointer-arith: Warn about anything that depends on the "size of" a function type or of "void"
# -Wcast-qual: For example, warn if a "const char *" is cast to an ordinary "char *"
# -Wstrict-overflow: Warns about cases where the compiler optimizes
#                    based on the assumption that signed overflow does not occur
# -Wwrite-strings: On C programs, give string constants the type "const char[length]"
# -Wswitch: Useful to catch missing siwtch cases
# -Wunreachable-code: Warn if the compiler detects that code will never be executed
# -Winit-self: Warn about uninitialized variables which are initialized with themselves
# -Wconversion: Warn for implicit conversions that may alter a value
ifeq ($(BUILD),release)
CFLAGS=-O3 -std=c++14 -DNDEBUG
else
CFLAGS=-O0 -g3 -std=c++14 -Wshadow -Wall -Wextra -Wold-style-cast -Wpedantic\
-Wfloat-equal -Wpointer-arith -Wcast-qual -Wstrict-overflow=5 -Wwrite-strings\
-Wswitch-default -Wswitch-enum -Wunreachable-code -Winit-self\
-Wno-unused-parameter
endif
# -Wconversion

# Find all source files
SRCS:=$(shell cd $(SRCDIR); find -name '*.cpp' -printf '%P\n' )
# Get all object files by substituting .cpp with .o
OBJECTS=$(SRCS:%.cpp=%.o)

# Files related to Bison
PARSER_NAME=parser
PARSER_DIR=parser
PARSER_Y=$(SRCDIR)/$(PARSER_DIR)/$(PARSER_NAME).y
PARSER_H=$(INCLDIR)/$(PARSER_DIR)/$(PARSER_NAME).h
PARSER_CPP=$(SRCDIR)/$(PARSER_DIR)/$(PARSER_NAME).cpp
PARSER_OUT=$(SRCDIR)/$(PARSER_DIR)/$(PARSER_NAME).output
PARSER_O=$(ODIR)/$(PARSER_DIR)/$(PARSER_NAME).o

# Files related to Yacc
LEXXER_NAME=lexxer
LEXXER_L=$(SRCDIR)/$(PARSER_DIR)/$(LEXXER_NAME).l
LEXXER_CPP=$(SRCDIR)/$(PARSER_DIR)/$(LEXXER_NAME).cpp
LEXXER_O=$(ODIR)/$(PARSER_DIR)/$(LEXXER_NAME).o

# Get object file paths
OBJ = $(patsubst %,$(ODIR)/%,$(OBJECTS))

#################################### RULES ####################################

all: $(EXECUTABLE)
	@echo 'Build successful'

compare: $(EXECUTABLE)
	@./$(EXECUTABLE) $(SAMPLE_TEST) > output1.txt
	@node $(SAMPLE_TEST).js > output2.txt
	diff output1.txt output2.txt
	@rm output1.txt output2.txt
	@echo 'Compared successfully'

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(SAMPLE_TEST)

# Create object files of source code
$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@echo Compiling $*
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $< $(LINKER)

# Create lexical analyzer
$(LEXXER_CPP): $(LEXXER_L)
	flex --outfile=$@ $<

$(PARSER_CPP): $(PARSER_Y)
	bison --yacc --defines=$(PARSER_H) -v --output=$@ $<

$(LEXXER_O): $(LEXXER_CPP)
	@mkdir -p $(ODIR)/$(PARSER_DIR)
	@$(CC) $^ -o $@ -c $(LINKER)

$(PARSER_O): $(PARSER_CPP)
	@mkdir -p $(ODIR)/$(PARSER_DIR)
	@$(CC) -Wno-write-strings $^ -o $@ -c ${LINKER}

$(EXECUTABLE): $(PARSER_O) $(LEXXER_O) $(OBJ)
	@echo 'Building interpreter'
	@$(CC) $(CFLAGS) $^ -o $@
	@echo ''

# Clean object files
clean:
	@rm -rf *.o
	@rm -rf $(ODIR)/*.o
	@rm -rf $(ODIR)
	@echo 'Object Files cleaned'
	@rm -rf $(LEXXER_CPP)
	@rm -rf $(PARSER_CPP)
	@rm -rf $(PARSER_OUT)
	@rm -rf $(PARSER_H)
	@echo 'Bison/Flex products cleaned'

# Clean app
distclean:
	@rm -rf *.out
	@rm -rf *.txt
	@rm -rf *.dot
	@rm -rf alpha_unparse.alpha
	@echo 'Files cleaned'

clear: clean distclean

viz: run
	xdot alpha_AST.dot

release: clean
	make -j 4 "BUILD=release"
	@echo 'Build successful for release mode'
