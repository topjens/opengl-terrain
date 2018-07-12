#include "load_texture.h"

GLuint load_texture(const char *filename)
{
		GLuint id = 0;
		SDL_Surface *image = IMG_Load(filename);

		if(!image) {
				("SDL_Image error in load_texture: %s\n", IMG_GetError());
				return 0;
		}

		int mode = GL_RGB;
		if(image->format->BytesPerPixel == 4)
				mode = GL_RGBA;

		glGenTextures(1, (GLuint *)&id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, mode, (GLsizei)image->w, (GLsizei)image->h,
					 0, mode, GL_UNSIGNED_BYTE, image->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(image);

		return id;
}
