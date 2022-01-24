.PHONY: all clean

CC=clang
CFLAGS+=-Wall -Werror -lpthread

all: a1q2 

a1q2: a1q2.c
	$(CC) $(CFLAGS) a1q2.c -o a1q2

clean:
	rm -f *.o a1q2
