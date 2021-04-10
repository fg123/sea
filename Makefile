CC=g++
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.cc)
DEPS=$(wildcard $(SRCDIR)/*.h)
OBJ = $(SRC:%.cc=%.o)
WARN=-Wall -Wextra -Werror
CPPFLAGS=-std=c++17 $(WARN) -g

$(SRCDIR)/%.o: $(SRCDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CXX) -o bin/sea $^ $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o
