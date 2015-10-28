EXE = test
SRC = $(wildcard *.c)

all: $(SRC)
	gcc -Wno-parentheses-equality $(SRC) -o $(EXE)

clean:
	$(RM) $(EXE)
