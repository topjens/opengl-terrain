#include "load_shader.h"

GLuint load_shaders(shader_info *shaders)
{
	char *shader_source;
	GLuint program = glCreateProgram();

	while(shaders->name)
	{
		printf("Loading shader %s\n", shaders->name);

		shader_source = read_file(shaders->name);
		if(!shader_source) {
			fprintf(stderr, "Could not open file %s\n", shaders->name);
			return 0;
		}

		GLuint shader = glCreateShader(shaders->type);
		glShaderSource(shader, 1, (const GLchar **)(&shader_source), 0);
		glCompileShader(shader);

		GLint compileSuccess = 0;
		GLchar compilerSpew[256];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
		if(!compileSuccess) {
			glGetShaderInfoLog(shader, sizeof(compilerSpew), 0, compilerSpew);
			fprintf(stderr, "Could not compile shader %s\nMessage = %s\nreturn value = %d\n", shaders->type, compilerSpew, compileSuccess);
			free(shader_source);
			return 0;
		}
	
		glAttachShader(program, shader);

		free(shader_source);
		shaders++;
	}

	glLinkProgram(program);
	
	GLint linkSuccess = 0;
	GLchar linkSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
	if(!linkSuccess) {
		glGetProgramInfoLog(program, sizeof(linkSpew), 0, linkSpew);
		fprintf(stderr, "Could not link\nMessage = %s\nreturn value = %d\n", linkSpew, linkSuccess);
		return 0;
	}

	return program;
}

char *read_file(const char *filename)
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
