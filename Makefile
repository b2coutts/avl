CC      = gcc
CFLAGS  = -pg -Ofast -fexpensive-optimizations -ffast-math -fno-exceptions \
          -std=c99

avl_demo: test.c avl.c avl.h
	$(CC) $(CFLAGS) test.c avl.c -o avl_demo
