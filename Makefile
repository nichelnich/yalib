CFLAGS=-g -I./include -Wall -DDEBUG_ASSERT -DDEBUG_MUTEX
BUILD_DIR=build

sources=$(wildcard src/*.c)

all: $(sources:.c=.o)
	$(AR) -r ./lib/libya.a $^
	
test: test/test_timer.o
	$(CC) -o test/test_timer $^ lib/libya.a -lpthread

.PHONY : clean
clean:
	- @rm ./src/*.o;
	- @rm ./lib/libya.a
	- @rm ./test/*.o
	- @rm ./test/test_timer
