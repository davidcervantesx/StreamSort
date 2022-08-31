CC=gcc
CCFLAGS=-O3 -Wall -Wextra -std=c11

.PHONY: all
all: stream_sort

stream_sort: stream_sort.c
	${CC} ${CCFLAGS} ${<} -o ${@}

.PHONY: debug
debug: stream_sort
	gdb ./stream_sort

.PHONY: leak-check
leak-check: stream_sort
	valgrind --leak-check=full ./stream_sort

.PHONY: clean
clean:
	rm stream_sort
