CFLAGS=-I./include -Wall
VPATH=src
BUILD_DIR=build

sources=$(wildcard src/*.c)

all: $(sources:.c=.o)
	$(AR) -r ./lib/libya.a $^

.PHONY : clean
clean:
	rm ./src/*.o; \
	rm ./lib/libya.a
