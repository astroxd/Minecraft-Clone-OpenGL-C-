#ifndef INVENTORY_H
#define INVENTORY_H


#include "Mesh.h"
#include "TextureManager.h"
#include "TextureAtlas.h"
#include "Input.h"
#include "BlockItem.h"
#include "Hotbar.h"
//struct GUIVertex {
//	glm::vec3 pos;
//	glm::vec2 UV;
//};



class Inventory : public Mesh<GUIVertex> {
public:
	Inventory();

	void GenerateMesh();
	void Transform();

	void SetVAO() override;
	void Draw() override;

	void Update();

private:
	float m_InventoryWidth = 176.0f;
	float m_InventoryHeight = 166.0f;
	glm::vec2 m_Scale = glm::vec2(3.0f);

	//UVs offset in pixel
	float m_xOffset = 0.0f;
	float m_yOffset = 90.0f;

	glm::vec2 m_SlotOffset = glm::vec2(0.0f);
	float m_SlotSize = 18.0f;

	int m_Rows = 4;
	int m_Columns = 9;

	bool isInventoryOpen = false;

	Shader m_Shader;
	glm::vec2 m_WindowSize = Window::GetInstance().getWindowSize();

	std::chrono::milliseconds m_LastButton;

	BlockItem m_InventoryItems;

private:
	void UpdateWindowSize();

	inline float GetScaledSize(const float size) const { return size * m_Scale.x; }

	inline float GetScaledWidth() const { return m_InventoryWidth * m_Scale.x; }
	inline float GetScaledHeight() const { return m_InventoryHeight * m_Scale.y; }
	inline float GetHorizontalTranslation() const { return (m_WindowSize.x / 2.0f) - (GetScaledWidth() / 2.0f); }
	inline float GetVerticalTranslation() const {
		return (m_WindowSize.y / 2.0f) - (GetScaledHeight() / 2.0f);
	}
	glm::vec3 GetTranslationVector() const;
	glm::vec3 GetSlotTranslationVector(int slotIndex) const;

	void HandleInput();

	//void ChangeSelectedSlot(int slotIndex);

	/// @param bottomLeft: Local pixel coordinates of bottom left corner
	/// @param topRight Local pixel coordinates of top right corner
	bool IsInSelectedArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);

};

#endif 
