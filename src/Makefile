CC=gcc

CFLAGS=-I../include `sdl2-config --cflags`
LIBS= `sdl2-config --libs` -lSDL_image -ldl -lm

all: opengl

opengl: main.o load_shader.o load_texture.o linear_algebra.o glad.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -r opengl *.o
