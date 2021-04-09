CC=g++
SRC=$(wildcard $(SRCDIR)/*.cc)
OBJ = $(SRC:%.c=%.o)
SRCDIR=src
WARN=-Wall -Wextra -Werror -Wno-return-void
CPPFLAGS=-std=c++11 $(WARN)

$(SRCDIR)/%.o: $(SRCDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CXX) -o bin/sea $^ $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o
