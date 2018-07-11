#ifndef _LOAD_SHADER_H__
#define _LOAD_SHADER_H__

#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"

char *read_file(char *filename);
int link_shaders(GLint fragment_handle, GLint vertex_handle);
GLint attach_shader(GLint type, char *filename);

#endif /* _LOAD_SHADER_H__ */
