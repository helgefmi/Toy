CC=g++
CPPFLAGS=-O3 -Wall -Wextra -pedantic -g2
SRC=src/pprinter_visitor.o src/ast.o src/parser.o src/main.o src/lexer.o src/exceptions.o
TARGET=toy

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

clean:
	rm -f src/*.o $(TARGET) toy.exe
