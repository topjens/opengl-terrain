#ifndef _SHADER_H__
#define _SHADER_H__

typedef struct {
		GLuint type;
		const char *name;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo *shaders);

#endif /* _SHADER_H__ */
