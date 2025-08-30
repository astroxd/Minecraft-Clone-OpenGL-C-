#include "TextureAtlas.h"
#include "Log.h"

TextureAtlas::TextureAtlas(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int HOffset, int VOffset)
	: Texture(image, texType, slot, format, pixelType), m_HorizontalOffset(HOffset), m_VerticalOffset(VOffset)
{
}

void TextureAtlas::BindAtlas(const std::string& shaderName, const char* uniform, const int unit) {
	Load(shaderName, uniform, unit);
	Bind();
}


std::vector<glm::vec2> TextureAtlas::GetUV(int x, int y, bool mirrored, float textureWidth, float textureHeight) const {
	x += m_HorizontalOffset;
	y += m_VerticalOffset;

	std::vector<glm::vec2> FaceUV;

	if (mirrored) {
		FaceUV = {
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, (y * textureHeight) / m_HeightImg),
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, ((y + 1) * textureHeight) / m_HeightImg),
			glm::vec2((x * textureWidth) / m_WidthImg, ((y + 1) * textureHeight) / m_HeightImg),
			glm::vec2((x * textureWidth) / m_WidthImg, (y * textureHeight) / m_HeightImg),

		};
	}
	else {
		FaceUV = {
			glm::vec2((x * textureWidth) / m_WidthImg, (y * textureHeight) / m_HeightImg),
			glm::vec2((x * textureWidth) / m_WidthImg, ((y + 1) * textureHeight) / m_HeightImg),
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, ((y + 1) * textureHeight) / m_HeightImg),
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, (y * textureHeight) / m_HeightImg),

		};
	}

	return FaceUV;
}

std::vector<glm::vec2> TextureAtlas::GetUV(float xOffset, float yOffset, float textureWidth, float textureHeight) const
{
	std::vector<glm::vec2> FaceUV = {
			glm::vec2(xOffset / m_WidthImg, yOffset / m_HeightImg),
			glm::vec2(xOffset / m_WidthImg, (yOffset + textureHeight) / m_HeightImg),
			glm::vec2((xOffset + textureWidth) / m_WidthImg, (yOffset + textureHeight) / m_HeightImg),
			glm::vec2((xOffset + textureWidth) / m_WidthImg,  yOffset / m_HeightImg),
	};

	return FaceUV;
}
