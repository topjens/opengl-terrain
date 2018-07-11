#include "glad/glad.h"

GLint attach_shader(int type, char *filename)
{
  char *mem = read_file(filename);
  if(!mem)
    fprintf(stderr, "Could not open file %s!\n", filename);

  GLuint handle = glCreateShader(type);
  glShaderSource(handle, 1, (const GLchar **)(&mem), 0);
  glCompileShader(handle);
  GLInt compileSuccess = 0;
  GLchar compilerSpew[256];

  glGetShaderiv(handle, GL_COMPILE_SATUS, &compileSuccess);
  if(!compileSuccess) {
    glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
    fprintf(stderr, "Could not compile shader %s\nMessage = %s\nreturn value = %d\n", filename, compilerSpew, compileSuccess);
    free(mem);
    return -1;
  }

  free(mem);
  return handle;
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
