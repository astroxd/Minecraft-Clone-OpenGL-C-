#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "Texture.h"

class TextureAtlas : public Texture {
public:

	TextureAtlas(const char* image, GLenum texType, GLenum slot, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE, int HOffset = 0, int VOffset = 0);

	void BindAtlas(const std::string& shaderName, const char* uName, const int index);

	std::vector<glm::vec2> GetUV(int x, int y, float textureWidth = 16.0f, float textureHeight = 16.0f) const;

private:
	int m_HorizontalOffset;
	int m_VerticalOffset;
};


#endif 

