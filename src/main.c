#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GLM_FORCE_RADIANS 1

#include <SDL2/SDL.h>

#include "glad/glad.h"

const int SCREEN_FULLSCREEN = 1;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
SDL_Window *window = NULL;
SDL_GLContext maincontext;

void sdl_die(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
	exit(EXIT_FAILURE);
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

	/* Disable depth test & face culling */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
}
