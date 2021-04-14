SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.cc)
DEPS=$(wildcard $(SRCDIR)/*.h)
OBJ = $(SRC:%.cc=%.o)
WARN=-Wall -Wextra -Werror -Wno-unused
CPPFLAGS=-std=c++17 $(WARN) -g 
LINKFLAGS=-lstdc++fs

$(SRCDIR)/%.o: $(SRCDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CXX) -o bin/sea $^ $(CPPFLAGS) $(LINKFLAGS)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o
