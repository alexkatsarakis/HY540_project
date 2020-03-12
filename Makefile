# Compiler
CC=g++

# Current directory
PROJECTDIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# Project structure
ODIR=obj
SRCDIR=src
TESTDIR=tests

# Name of the executable file
EXECUTABLE=interpreter.out

# Name of a simple test file that can be used as input
SAMPLE_TEST=$(TESTDIR)/add.alpha

# Specify the include directories
INCLDIR=-I$(PROJECTDIR)$

# Set compilation flags
# -Wshadow : Warn whenever a local variable or type declaration shadows another
#            variable, parameter, type or when a built-in function is shadowed
CFLAGS=-std=c++14 -Wshadow -Wall -Wold-style-cast -Wpedantic $(INCLDIR)

# Find all source files
SRCS:=$(shell cd $(SRCDIR); ls *.cpp)
# Get all object files by substituting .cpp with .o
OBJECTS=$(SRCS:%.cpp=%.o)

# Files related to Bison
PARSER_NAME=parser
PARSER_Y=$(SRCDIR)/$(PARSER_NAME).y
PARSER_CPP=$(SRCDIR)/$(PARSER_NAME).cpp
PARSER_OUT=$(SRCDIR)/$(PARSER_NAME).output
PARSER_O=$(ODIR)/$(PARSER_NAME).o

# Files related to Yacc
LEXXER_NAME=lexxer
LEXXER_L=$(SRCDIR)/$(LEXXER_NAME).l
LEXXER_CPP=$(SRCDIR)/$(LEXXER_NAME).cpp
LEXXER_O=$(ODIR)/$(LEXXER_NAME).o

# Get object file paths
OBJ = $(patsubst %,$(ODIR)/%,$(OBJECTS))

#################################### RULES ####################################

all: $(EXECUTABLE)
	@echo 'Build successful'

run: $(EXECUTABLE)
	./$(EXECUTABLE) < $(SAMPLE_TEST)

# Create object files of source code
$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@echo Compiling $*
	@mkdir -p $(ODIR)
	$(CC) -std=c++14 -c -o $@ $< -g

# Create lexical analyzer
$(LEXXER_CPP): $(LEXXER_L)
	flex --outfile=$@ $<

$(PARSER_CPP): $(PARSER_Y)
	bison --yacc --defines=$(SRCDIR)/parser.h -v --output=$@ $<

$(LEXXER_O): $(LEXXER_CPP)
	@mkdir -p $(ODIR)
	$(CC) $^ -o $@ -c

$(PARSER_O): $(PARSER_CPP)
	@mkdir -p $(ODIR)
	$(CC) $^ -o $@ -c -Wno-write-strings

$(EXECUTABLE): $(PARSER_O) $(LEXXER_O) $(OBJ)
	@echo 'Creating interpreter'
	$(CC) $(CFLAGS) $^ -o $@

# Clean object files
clean:
	@rm -rf *.o
	@rm -rf $(ODIR)/*.o
	@rm -rf $(ODIR)
	@echo 'Object Files cleaned'
	@rm -rf $(LEXXER_CPP)
	@rm -rf $(PARSER_CPP)
	@rm -rf $(PARSER_OUT)
	@echo 'Bison/Flex products cleaned'

# Clean app
distclean:
	@rm -rf *.out
	@rm -rf *.txt
	@echo 'Files cleaned'

clear: clean distclean