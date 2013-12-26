CC      = gcc
CFLAGS  = -Ofast -fexpensive-optimizations -ffast-math -fno-exceptions -std=c99

avl_demo: test.c obj/avl.o
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast test.c avl.c -o avl_demo

obj/avl.o: avl.c avl.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o obj/avl.o avl.c

clean:
	rm obj/avl.o avl_demo

.PHONY: clean
