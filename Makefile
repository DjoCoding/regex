SRC := $(shell find . -name '*.c')

main: $(SRC)
	@gcc $(SRC) -o main -ggdb2 -Wall -I./

viz: main
	./main
	dot -Tpng nfa.dot -o nfa.png