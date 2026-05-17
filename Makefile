CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/allocator.c src/freelist.c src/utils.c

all:
	gcc $(CFLAGS) $(SRC) pro.c -o pro

run:
	./pro

test:
	gcc $(CFLAGS) $(SRC) tests/test_basic.c -o test_basic
	./test_basic

stress:
	gcc $(CFLAGS) $(SRC) tests/test_stress.c -o stress
	./stress

clean:
	rm -f pro test_basic stress