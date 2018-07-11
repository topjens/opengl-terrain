#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GLM_FORCE_RADIANS 1

#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "load_shader.h"

const int SCREEN_FULLSCREEN = 1;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
SDL_Window *window = NULL;
SDL_GLContext maincontext;
unsigned char *map;

void sdl_die(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
	exit(EXIT_FAILURE);
}

int ogl_tex_new(unsigned int size_x, unsigned int size_y, int filter, int repeat, int type1, int type2, unsigned char *data, int type3)
{
	int id = 0;

	glGenTextures(1, (GLuint *)&id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (filter == GL_LINEAR_MIPMAP_LINEAR) ? GL_LINEAR : filter);

	glTexImage2D(GL_TEXTURE_2D, 0, type1, size_x, size_y, 0, type2, type3, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return id;
}

void init_screen(const char *caption)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		sdl_die("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	/*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);*/
	if(SCREEN_FULLSCREEN) {
		window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED, 0, 0,
					  SDL_WINDOW_FULLSCREEN_DESKTOP |
					  SDL_WINDOW_OPENGL);
	} else {
		window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
					  SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	}
	if(!window)
		sdl_die("Couldn't create window");

	maincontext = SDL_GL_CreateContext(window);
	if(!maincontext)
		sdl_die("Failed to create OpenGL context");

	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	/* 
	 *  Use V-Sync
	 *  This does not seem to work, solved in SDL event loop
	 */
	SDL_GL_SetSwapInterval(1);

	/* Enable depth test & face culling */
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLint vertex_handle = attach_shader(GL_VERTEX_SHADER, "../shader/vertex.shader");
	GLint fragment_handle = attach_shader(GL_FRAGMENT_SHADER, "../shader/fragment.shader");
	link_shaders(fragment_handle, vertex_handle);

	int width = 800;
	int height = 600;

	map = malloc(width*height*4);
	
	int i, j;
	for(j = 0; j < height; j++)
		for(i = 0; i < width; i++) {
			float x = (float)i/(float)width;
			float y = (float)j/(float)height;
			float h = (sin(4*M_PI*x)+sin(4*M_PI*y)+sin(16*M_PI*x)*sin(16*M_PI*y))*0.125+0.5;
			((float *)map)[i+j*width]=h;
		}

	int tex_heightmap = ogl_tex_new(width, height, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_LUMINANCE16, GL_LUMINANCE, map, GL_FLOAT);

	glGenBuffers(1, (GLuint *)(&vbo));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sizeof(float)*vert.size(), &vert[0], GL_DYNAMIC_DRAW);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0);
	glRotatef(180, 1, 0, 0);
}

int main(int argc, char *argv[])
{
	init_screen("OpenGL 4.5");
	SDL_Event event;
	
	bool quit = false;

	uint32_t ticks = SDL_GetTicks(), lastticks = 0;
	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT);
		ticks = SDL_GetTicks();
		if(((ticks*10-lastticks*10)) < 167)
			SDL_Delay((167-((ticks*10-lastticks*10)))/10);
		lastticks = SDL_GetTicks();
		SDL_GL_SwapWindow(window);
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
			} else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
		}
	}

	free(map);
}
