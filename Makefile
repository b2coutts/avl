CC      = gcc
CFLAGS  = -Ofast -fexpensive-optimizations -ffast-math -fno-exceptions -std=c99 -Wall

all: bin/avl_pg bin/avl_demo
	

bin/avl_pg: test.c obj/avl.o obj/test.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -pg obj/test.o obj/avl.o -o bin/avl_pg

bin/avl_demo: test.c obj/avl.o obj/test.o
	@mkdir -p bin
	$(CC) $(CFLAGS) obj/test.o obj/avl.o -o bin/avl_demo

obj/%.o: %.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -rf obj/*.o bin/avl_demo

.PHONY: clean, all
