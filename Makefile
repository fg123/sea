CC=g++
SRC=$(wildcard $(SRCDIR)/*.cc)
OBJ = $(SRC:%.cc=%.o)
SRCDIR=src
WARN=-Wall -Wextra -Werror
CPPFLAGS=-std=c++14 $(WARN) -g

$(SRCDIR)/%.o: $(SRCDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CXX) -o bin/sea $^ $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o
