#include "load_shader.h"

GLint attach_shader(GLint type, char *filename)
{
	char *mem = read_file(filename);
	if(!mem)
		fprintf(stderr, "Could not open file %s!\n", filename);

	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar **)(&mem), 0);
	glCompileShader(handle);
	GLint compileSuccess = 0;
	GLchar compilerSpew[256];

	glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
	if(!compileSuccess) {
		glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
		fprintf(stderr, "Could not compile shader %s\nMessage = %s\nreturn value = %d\n", filename, compilerSpew, compileSuccess);
		free(mem);
		return -1;
	}

	free(mem);
	return handle;
}

int link_shaders(GLint fragment_handle, GLint vertex_handle)
{
	int i;
	GLint program_handle = glCreateProgram();
	
	glAttachShader(program_handle, fragment_handle);
	glAttachShader(program_handle, vertex_handle);

	glLinkProgram(program_handle);
	GLint linkSuccess = 0;
	GLchar linkSpew[256];

	glGetProgramiv(program_handle, GL_LINK_STATUS, &linkSuccess);
	if(!linkSuccess) {
		glGetProgramInfoLog(program_handle, sizeof(linkSpew), 0, linkSpew);
		fprintf(stderr, "Could not link\nMessage = %s\nreturn value = %d\n", linkSpew, linkSuccess);
		return 0;
	}
	return 1;
}

/*
 * Reads file `Filename' and returns its contents as a char *
 */
char *read_file(char *filename)
{
	FILE *fp = fopen(filename, "r");

	if(!fp)
		return NULL;

	/* Determine size of file and allocate buffer */
	fseek(fp, 0L, SEEK_END);
	int fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *mem = (char *)calloc(fsize + 1, 1);

	fread(mem, 1, fsize, fp);

	fclose(fp);

	return mem;
}
