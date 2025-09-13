#ifndef INVENTORY_H
#define INVENTORY_H


#include "Mesh.h"
#include "TextureManager.h"
#include "TextureAtlas.h"
#include "Input.h"
#include "BlockItem.h"
#include "Hotbar.h"


class Inventory : public Mesh<GUIVertex> {
public:
	Inventory();

	void GenerateMesh();

	void Draw() override;

	void Update();

	Hotbar HotBar;

	inline static InventoryItem s_SelectedHotbarItem;

private:
	const float m_InventoryWidth = 176.0f;
	const float m_InventoryHeight = 166.0f;
	glm::vec2 m_Scale = glm::vec2(3.0f);

	//UVs offset in pixel
	const float m_xOffset = 0.0f;
	const float m_yOffset = 90.0f;

	glm::vec2 m_SlotOffset = glm::vec2(0.0f);
	const float m_SlotSize = 18.0f;

	int m_Rows = 4;
	int m_Columns = 9;

	bool m_IsInventoryOpen = false;

	Shader m_Shader;
	glm::vec2 m_WindowSize = Window::GetInstance().getWindowSize();

	std::chrono::milliseconds m_LastButton;

	std::vector<InventoryItem> m_Items;
	BlockItem m_InventoryItems{ m_Scale };

	int m_HoveredSlot = -1;
	bool m_IsItemPicked = false;
	int m_PickedSlot = -1;

	int m_SelectedHotbarSlot = 0;



private:
	void SetVAO() override;
	void Transform();

	void UpdateWindowSize();

	inline float GetScaledSize(const float size) const { return size * m_Scale.x; }
	inline float GetScaledWidth() const { return m_InventoryWidth * m_Scale.x; }
	inline float GetScaledHeight() const { return m_InventoryHeight * m_Scale.y; }
	inline float GetHorizontalTranslation() const { return (m_WindowSize.x / 2.0f) - (GetScaledWidth() / 2.0f); }
	inline float GetVerticalTranslation() const { return (m_WindowSize.y / 2.0f) - (GetScaledHeight() / 2.0f); }
	inline glm::vec3 GetTranslationVector() const { return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0); };
	glm::vec3 GetSlotTranslationVector(const int slotIndex) const;

	void HandleInput();

	/// @param bottomLeft: Local pixel coordinates of bottom left corner
	/// @param topRight Local pixel coordinates of top right corner
	bool IsInSelectedArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);


	std::vector<glm::vec3> CreateItemOffsets();
	void SetItemOffsets(const std::vector<glm::vec3>& offsets);

	void SwapItems(const int i, const  int j);
	void SendItems();

	void SendHotbarItems();
	void ChangeSelectedHotbarSlot(const int hotbarSlotIndex);
};

#endif 
