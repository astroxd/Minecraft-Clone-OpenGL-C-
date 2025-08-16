#include"Texture.h"
#include "Log.h"


Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	: m_Type(texType)
{
	// Stores the number of color channels of the image
	int numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* data = stbi_load(image, &m_WidthImg, &m_HeightImg, &numColCh, 0);

	if (!data) {
		LOG_ERROR("{0} failed to load", image);
		return;
	}

	LOG_INFO("{0}: WIDTH {1}, HEIGHT {2}", image, m_WidthImg, m_HeightImg);

	// Generates an OpenGL texture object
	glGenTextures(1, &m_ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, m_ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	glTexImage2D(texType, 0, GL_RGBA, m_WidthImg, m_HeightImg, 0, format, pixelType, data);
	// Generates MipMaps
	glGenerateMipmap(texType);

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(data);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(texType, 0);
}

void Texture::Load(const std::string& shaderName, const char* uniform, GLuint unit) {
	Shader shader = ShaderManager::GetShader(shaderName);
	shader.Activate();
	shader.SetInt(uniform, unit);
}

void Texture::Bind() {
	glBindTexture(m_Type, m_ID);
}

void Texture::Unbind() {
	glBindTexture(m_Type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &m_ID);
}