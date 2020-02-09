IDIR=includes
ODIR=objects
CDIR=./
OUT=main
_DEPS=
_OBJ=main.o

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

WFLAGS=-Wall -Wextra -Wfloat-equal -Wshadow -Wpointer-arith\
-Wcast-align -Wcast-qual -Wstrict-overflow=5 -Wwrite-strings\
-Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code\
-Winit-self -Wundef

CXX=g++
CXXFLAGS=$(WFLAGS) -I$(IDIR) -std=c++11

.PHONY: clean
default: debug

debug: CXXFLAGS+=-g3 -O0
debug: out
release: CXXFLAGS+=-O3 -DNDEBUG
release: out

$(ODIR)/%.o: $(CDIR)/%.cpp $(DEPS)
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

out: $(OBJ)
	@$(CXX) -o $(OUT) $^ $(CXXFLAGS)

clean:
	rm -f $(ODIR)/*.o
	rm -f $(OUT)
