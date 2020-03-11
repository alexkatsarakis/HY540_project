# Compiler
CC=g++

# Current directory
PROJECTDIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# Project structure
ODIR=obj
SRCDIR=src

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

# Get object file paths
OBJ = $(patsubst %,$(ODIR)/%,$(OBJECTS))

#################################### RULES ####################################

all: interpreter.out
	@echo 'Build successful'

# Create object files of source code
$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@echo Compiling $*
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $< -g

interpreter.out: $(OBJ)
	@echo 'Creating interpreter'
	$(CC) $(CFLAGS) $^ -o $@ -g

# Clean object files
clean:
	@rm -rf *.o
	@rm -rf $(ODIR)/*.o
	@rm -rf $(ODIR)
	@echo 'Object Files cleaned'

# Clean app
distclean:
	@rm -rf *.out
	@echo 'Executables cleaned'