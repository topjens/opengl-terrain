#ifndef _LOAD_SHADER_H__
#define _LOAD_SHADER_H__

#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"

typedef struct {
	GLuint type;
	const char *name;
} shader_info;

GLuint load_shaders(shader_info *shaders);
char *read_file(const char *filename);

#endif /* _LOAD_SHADER_H__ */
