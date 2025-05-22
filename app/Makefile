CC=clang
CFLAGS=-Wall -Wextra -std=c99
LDFLAGS=-framework OpenGL -framework GLUT -lm

all: virtual_gallery

virtual_gallery: main.o
	$(CC) $(CFLAGS) -o $@ main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f virtual_gallery *.o
