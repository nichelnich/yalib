CFLAGS=-I./include -Wall -DDEBUG_ASSERT -DDEBUG_MUTEX
BUILD_DIR=build

sources=$(wildcard src/*.c)

all: $(sources:.c=.o)
	$(AR) -r ./lib/libya.a $^

.PHONY : clean
clean:
	rm ./src/*.o; \
	rm ./lib/libya.a
