SRC := $(shell find . -name '*.c')

main: $(SRC)
	@gcc $(SRC) -o main -ggdb2 -Wall -I./ 