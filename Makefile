#includes directory
IDIR=includes
#objects directory
ODIR=objects
#source directory
CDIR=source
#executable name
OUT=marmotes

#add .h dependency names here
_DEPS=
#add .o linker object names here
_OBJ=$(OUT)_parser.o $(OUT)_scanner.o main.o

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

WFLAGS=-Wall -Wextra -Wfloat-equal -Wshadow -Wpointer-arith\
-Wcast-align -Wcast-qual -Wstrict-overflow=5 -Wwrite-strings\
-Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code\
-Winit-self -Wundef

LEX=flex
YACC=bison
CXX=g++
LFLAGS=
YFLAGS=--yacc
CXXFLAGS=$(WFLAGS) -I$(IDIR) -std=c++11

.PHONY: clean
default: debug

debug: CXXFLAGS+=-g3 -O0
debug: out
release: CXXFLAGS+=-O3 -DNDEBUG
release: out
conflicts: YFLAGS+= -v
conflicts: CXXFLAGS+=-g3 -O0
conflicts: out

$(CDIR)/$(OUT)_lexer.cpp: $(OUT)_lexer.l
	$(LEX) -o $@ $< $(LFLAGS)

$(IDIR)/$(OUT)_parser.h $(CDIR)/$(OUT)_parser.cpp &: $(OUT)_parser.y
	$(YACC) --defines=$(IDIR)/$(OUT)_parser.h -o $(CDIR)/$(OUT)_parser.cpp $< $(YFLAGS)

$(ODIR)/%.o: $(CDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

out: $(OBJ)
	$(CXX) -o $(OUT) $^ $(CXXFLAGS)

clean:
	rm -f $(CDIR)/$(OUT)_lexer.cpp
	rm -f $(IDIR)/$(OUT)_parser.h
	rm -f $(CDIR)/$(OUT)_parser.cpp
	rm -f $(ODIR)/*.o
	rm -f $(OUT)
