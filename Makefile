CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/avl.c src/metrics.c src/queries.c src/ui.c src/main.c
OBJ = $(SRC:.c=.o)
EXEC = cidade_inteligente

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o cidade_inteligente.exe 2>nul || exit 0

run: all
	./$(EXEC)
