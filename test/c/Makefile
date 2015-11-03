EXE = test
SRC = $(wildcard *.c)

# -Wno-parentheses-equality disables a (seemingly unnecessary) warning from seatest.c
# -I. tells the compiler to look for system / angled includes in the current folder
all: $(SRC)
	gcc -Wno-parentheses-equality -I. $(SRC) -o $(EXE)

clean:
	$(RM) $(EXE)
