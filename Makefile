test: test.c test.h
	clang --std=gnu11 -Wall -Wextra -Werror -pedantic test.c -o test
	./test