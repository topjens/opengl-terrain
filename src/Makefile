CC=gcc

CFLAGS=-I../include `sdl2-config --cflags`
LIBS= `sdl2-config --libs` -ldl -lm

all: opengl

opengl: main.o load_shader.o glad.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -r opengl *.o
