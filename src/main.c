#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GLM_FORCE_RADIANS 1

#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "load_shader.h"
#include "load_texture.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))

const int SCREEN_FULLSCREEN = 1;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

GLuint program;

GLuint mode = GL_POINTS;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

GLuint tex_heightmap;
GLuint tex_terrain;

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

	int img_flags = IMG_INIT_JPG|IMG_INIT_PNG;
	int img_ret = IMG_Init(img_flags);
	if((img_ret & img_flags) != img_flags)
		sdl_die("Couldn't initialize SDL_Image");
	atexit(IMG_Quit);
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
}

static void opengl_error_callback(GLenum source, GLenum type, GLuint id,
								  GLenum severity, GLsizei length,
								  const GLchar *message, const void *param)
{
	(void)source;
	(void)type;
	(void)id;
	(void)severity;
	(void)length;
	(void)param;

	fprintf(stderr, "%s\n", message);
	if(severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		abort();
	}
}

void init_gl(void)
{
	/* Setup error callback */
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengl_error_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
						  0, NULL, true);

	/* Enable depth test & face culling */
	/*glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);*/

	/* Set the clear screen color */
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Load vertices */
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[6][2] = {
		{-0.90, -0.90}, /* Triangle 1 */
		{ 0.85, -0.90},
		{-0.90,  0.85},
		{ 0.90, -0.85}, /* Triangle 2 */
		{ 0.90,  0.90},
		{-0.85,  0.90}
	};

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
				 vertices, GL_STATIC_DRAW);

	shader_info shaders[] = {
		{GL_VERTEX_SHADER, "../shader/vertex.shader"},
		{GL_FRAGMENT_SHADER, "../shader/fragment.shader"},
		{GL_NONE, NULL}
	};
		
	program = load_shaders(shaders);
	glUseProgram(program);
	
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
						  GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
}

void gl_draw(void)
{	
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(mode, 0, NumVertices);

	glFlush();
}

int main(int argc, char *argv[])
{
	init_screen("OpenGL 4.5");
	init_gl();
		
	SDL_Event event;
	
	bool quit = false;

	uint32_t ticks = SDL_GetTicks(), lastticks = 0;
	while(!quit) {
		ticks = SDL_GetTicks();
		if(((ticks*10-lastticks*10)) < 167)
			SDL_Delay((167-((ticks*10-lastticks*10)))/10);
		lastticks = SDL_GetTicks();
		gl_draw();
		SDL_GL_SwapWindow(window);
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
			} else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_SPACE:
					mode++;
					if(mode > 6)
						mode = GL_POINTS;
					break;
				default:
					break;
				}
			}
		}
	}
}
