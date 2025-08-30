#ifndef HOTBAR_H
#define HOTBAR_H

#include <glm/glm.hpp>

#include "Mesh.h"
#include "TextureManager.h"
#include "TextureAtlas.h"
#include "Input.h"

struct GUIVertex {
	glm::vec3 pos;
	glm::vec2 UV;
};

struct SlotSelector {

	float width = 24.0f;
	float height = 24.0f;


	glm::vec3 vertices[4] = {
		 glm::vec3(0 , 0 , 0),
		 glm::vec3(0 , height, 0),
		 glm::vec3(width, height, 0),
		 glm::vec3(width, 0 , 0),
	};

	std::vector<glm::vec2> UV;

	SlotSelector() {
		UV.reserve(4);
		GenerateUV();
	}

	void GenerateUV() {
		UV = static_cast<TextureAtlas&>(TextureManager::GetTexture("widget.png")).GetUV(0.0f, 210.0f, width, height);
	}

	void TransformVertices(const glm::vec3& offset) {
		for (int i = 0; i < 4; i++)
		{
			vertices[i] += offset;
		}
	}

};

enum Slots {
	SLOT0 = 0,
	SLOT1,
	SLOT2,
	SLOT3,
	SLOT4,
	SLOT5,
	SLOT6,
	SLOT7,
	SLOT8,
};

class Hotbar : public Mesh<GUIVertex> {
public:
	Hotbar();

	void GenerateMesh();
	void Transform();

	void SetVAO() override;
	void Draw() override;

	void Update();

private:
	float m_HotBarWidth = 182.0f;
	float m_HotBarHeight = 22.0f;
	glm::vec2 m_Scale = glm::vec2(2.0f, 2.0f);

	//UVs offset in pixel
	float m_xOffset = 0.0f;
	float m_yOffset = 234.0f;

	SlotSelector slotSelector;
	float m_Offset = 0.0f;
	float m_SlotSize = 20.0f;

	Shader m_Shader;
	glm::vec2 m_WindowSize = Window::GetInstance().getWindowSize();

	std::chrono::milliseconds m_LastButton;

private:
	void UpdateWindowSize();

	inline float GetScaledWidth() const { return m_HotBarWidth * m_Scale.x; }
	inline float GetScaledHeight() const { return m_HotBarHeight * m_Scale.y; }
	inline float GetHorizontalTraslation() const { return (m_WindowSize.x / 2.0f) - (GetScaledWidth() / 2.0f); }
	inline float GetVerticalTraslation() const {
		float VerticalTraslation = 10.0f;
		return VerticalTraslation;
	}
	glm::vec3 GetTraslationVector() const;

	void HandleInput();

	void ChangeSelectedSlot(int slotIndex);

};

#endif 
