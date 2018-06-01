CC=gcc
SRC=src
WARN=-Wall -Wextra -Werror -Wstrict-prototypes -Wmissing-prototypes -Wno-return-void -Wimplicit-function-declaration
CFLAGS=-std=c99 $(WARN)

_DEPS=*.h
DEPS=$(patsubst %,$(SRC)/%,$(_DEPS))

_OBJ=main.o
OBJ=$(patsubst %,$(SRC)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o seac $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(SRC)/*.o 
