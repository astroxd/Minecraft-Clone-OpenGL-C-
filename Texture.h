#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <stb/stb_image.h>

#include "shaderClass.h"
#include <vector>

class Texture {
public:

	GLuint ID;
	GLenum type;
	
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	Texture(std::vector<std::string> faces, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	
	void Bind();
	void Unbind();
	void Delete();

};

#endif;
