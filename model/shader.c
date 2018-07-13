#include "shader.h"

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

GLuint LoadShaders(ShaderInfo *shaders)
{
		char *shader_source;
		GLuint program = glCreateProgram();
		
		while(shaders->name)
		{
				printf("Loading shader %s\n", shaders->name);

				shader_source = read_file;
				if(!shader_source) {
						fprintf(stderr, "Could not open file %s\n", filename);
						return 0;
				}

				GLuint shader = glCreateShader(shaders->type);
				glShaderSource(shader, 1, (const GLchar **)(&shader_source), 0);
				glCompileShader(shader);

				glAttachShader(program, shader);

				free(shader_source);
				shaders++;
		}

		glLinkProgram(program);

		return program;
}
