#includes directory
IDIR=includes
#objects directory
ODIR=objects
#source directory
CDIR=source
#executable name
OUT=Marmotes
OUTLEXER=$(OUT)Lexer
OUTPARSER=$(OUT)Parser

#add .h dependency names here
_DEPS=
#add .o linker object names here
_OBJ=$(OUTPARSER).o $(OUTLEXER).o main.o
# _OBJ=$(OUTLEXER).o main.o

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

$(CDIR)/$(OUTLEXER).cpp: $(OUTLEXER).l
	$(LEX) -o $@ $< $(LFLAGS)

$(IDIR)/$(OUTPARSER).h $(CDIR)/$(OUTPARSER).cpp &: $(OUTPARSER).y
	$(YACC) --defines=$(IDIR)/$(OUTPARSER).h -o $(CDIR)/$(OUTPARSER).cpp $< $(YFLAGS)

$(ODIR)/%.o: $(CDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

out: $(OBJ)
	$(CXX) -o $(OUT) $^ $(CXXFLAGS)

clean:
	rm -f $(CDIR)/$(OUTLEXER).cpp
	rm -f $(IDIR)/$(OUTPARSER).h
	rm -f $(CDIR)/$(OUTPARSER).cpp
	rm -f $(ODIR)/*.o
	rm -f $(OUT)
