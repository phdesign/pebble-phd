EXE = test
SRC = $(wildcard *.c) ../../src/config.c ../../src/time-layer.c ../../src/weather-layer.c ../../src/utils.c

# -Wno-parentheses-equality disables a (seemingly unnecessary) warning from seatest.c
# -I. tells the compiler to look for system / angled includes in the current folder
all: $(SRC)
	gcc -Wno-parentheses-equality -I. -I../../src/ -g $(SRC) -o $(EXE)

clean:
	$(RM) $(EXE)
