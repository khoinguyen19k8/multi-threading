OBJS	= spinlock.o 
SOURCE	= spinlock.c
HEADER	= 
OUT	= spinlock
CC	 = clang
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

spinlock: spinlock.c
	$(CC) $(FLAGS) $^

clean:
	rm -f $(OBJS) $(OUT)
