#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb/stb_image.h>
#include <vector>

#include "ShaderManager.h"

class Texture {
public:

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);

	virtual ~Texture() {
		Delete();
	};

	void Load(const std::string& shaderName, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();

protected:
	GLuint m_ID;
	GLenum m_Type;

	int m_WidthImg;
	int m_HeightImg;
};

#endif;
