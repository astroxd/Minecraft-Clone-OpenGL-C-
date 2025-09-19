#include "Inventory.h"

#include "Utils.h"

Inventory::Inventory()
	: m_WindowResizeHandler([this](const WindowResizeEvent& e) { OnWindowResize(e); })
	, m_BlockPlacedHandler([this](const BlockPlacedEvent& e) {OnBlockPlaced(e); })
{
	LOG_INFO("Inventory Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");
	Events::Subscribe(m_WindowResizeHandler);
	Events::Subscribe(m_BlockPlacedHandler);

	//36 = Number of Slots
	//1 = The extra slot for the picked item
	//The slot 0 is in the top left corner
	//The slot 35 is the bottom right cornero of hotbar
	m_Items.reserve(36 + 1);
	for (int i = 0; i < 36 + 1; i++)
	{
		m_Items.emplace_back(InventoryItem{ 0, 0, i });
	}

	for (auto& item : InventoryItems)
	{
		m_Items[item.slot] = item;
	}

	s_SelectedHotbarItem = &m_Items[m_SelectedHotbarSlot + 27];

	SendItems();
	SetItemOffsets(CreateItemOffsets());

	HotBar.SetInventoryOpen(m_IsInventoryOpen);
	SendHotbarItems();

	GenerateMesh();
}

void Inventory::GenerateMesh() {
	//Temp Values for generating a black texture used for slot highlighter
	int m_WidthImg = 256;
	int m_HeightImg = 256;
	int countIndices = 0;

	//! Slot Highlighter
	m_Vertices.push_back(GUIVertex{ glm::vec3(7,65, 0),  glm::vec2(0, 0) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(7,83, 0) , glm::vec2(0,16.0f / m_HeightImg) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(25,83, 0) , glm::vec2(16.0f / m_WidthImg, 16.0f / m_HeightImg) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(25 ,65, 0) , glm::vec2(16.0f / m_WidthImg, 0) });

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	countIndices += 4;
	////////////////////////////////////////////

	//! Inventory
	std::vector<glm::vec2> FaceUV = static_cast<TextureAtlas&>(TextureManager::GetTexture("inventory.png")).GetUV(m_xOffset, m_yOffset, m_InventoryWidth, m_InventoryHeight);

	m_Vertices.push_back(GUIVertex{ glm::vec3(0, 0, 0),  FaceUV[0] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(0, m_InventoryHeight, 0) , FaceUV[1] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(m_InventoryWidth, m_InventoryHeight, 0) , FaceUV[2] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(m_InventoryWidth, 0, 0) , FaceUV[3] });

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	countIndices += 4;
	////////////////////////////////////////////

	SetVAO();
	Transform();
}

void Inventory::Update() {
	HotBar.Update();

	glm::vec2 mousePos = Input::getMousePosition();

	m_Shader.Activate();

	HandleInput();

	if (!m_IsInventoryOpen) return;

	if (IsInSelectedArea(glm::vec2(7.0f, 30.0f), glm::vec2(169.0f, 83.0f)) ||
		IsInSelectedArea(glm::vec2(7.0f, 7.0f), glm::vec2(169.0f, 25.0f)))
	{
		float localY = mousePos.y - (GetVerticalTranslation() + GetScaledSize(83.0f));
		float localX = mousePos.x - (GetHorizontalTranslation() + GetScaledSize(7.0f));

		int ySlotIndex = std::min(int(localY / GetScaledSize(m_SlotSize)), m_Rows - 1);
		int xSlotIndex = std::min(int((localX / GetScaledSize(m_SlotSize))), m_Columns - 1);

		m_HoveredSlot = xSlotIndex + ySlotIndex * 8 + ySlotIndex;

		if (ySlotIndex >= 3) {
			m_SlotOffset = glm::vec2(xSlotIndex, -ySlotIndex) * m_SlotSize - glm::vec2(0.0f, 4.0f);
		}
		else {
			m_SlotOffset = glm::vec2(xSlotIndex, -ySlotIndex) * m_SlotSize;
		}

		m_Shader.SetVec2("slotOffset", m_SlotOffset);
		m_Shader.SetBool("highlightSlot", true);
	}
	else {
		m_Shader.SetBool("highlightSlot", false);
		m_HoveredSlot = -1;
	}

	//If Item is Picked update its position every frame
	if (m_IsItemPicked) {
		m_InventoryItems.UpdateTransform(CreateItemOffsets());
	}
}

void Inventory::HandleInput() {

	if (Input::isKeyPressed(Key::E)) {
		std::chrono::milliseconds time = Utils::GetMs();
		if ((time - m_LastButton).count() > 20) {
			m_IsInventoryOpen = !m_IsInventoryOpen;
			m_Shader.Activate();
			m_Shader.SetBool("isInventoryOpen", m_IsInventoryOpen);
			HotBar.SetInventoryOpen(m_IsInventoryOpen);

			if (!m_IsInventoryOpen) m_Shader.SetBool("highlightSlot", false);
		}
		m_LastButton = time;
	}
	else if (Input::isKeyPressed(Key::Escape)) {
		m_IsInventoryOpen = false;
		m_Shader.Activate();
		m_Shader.SetBool("isInventoryOpen", m_IsInventoryOpen);
		m_Shader.SetBool("highlightSlot", false);
		HotBar.SetInventoryOpen(m_IsInventoryOpen);
	}
	else if (Input::isKeyPressed(Key::Q)) {
		if (m_IsInventoryOpen) {
			if (m_HoveredSlot == -1) return;
			m_Items[m_HoveredSlot] = InventoryItem{ 0, 0, m_HoveredSlot };
		}
		else {
			m_Items[m_SelectedHotbarSlot + 27] = InventoryItem{ 0, 0, m_SelectedHotbarSlot + 27 };
		}

		SendItems();
		SetItemOffsets(CreateItemOffsets());

		SendHotbarItems();
	}

	if (Input::isMouseButtonPressed(Mouse::Button0)) {
		std::chrono::milliseconds time = Utils::GetMs();
		if ((time - m_LastButton).count() > 20) {
			if (!m_IsItemPicked && m_HoveredSlot != -1) {
				if (m_Items[m_HoveredSlot].id > 0) {
					m_PickedSlot = 36;
					m_IsItemPicked = true;

					SwapItems(m_HoveredSlot, m_PickedSlot);

					SendItems();
					SetItemOffsets(CreateItemOffsets());

					SendHotbarItems();
				}
			}
			else if (m_IsItemPicked) {

				//Outside Inventory Area
				if (m_HoveredSlot == -1) {
					//! Should Drop Item
					m_Items[m_PickedSlot] = InventoryItem{ 0, 0, m_PickedSlot };
				}
				else {
					SwapItems(m_HoveredSlot, m_PickedSlot);
				}

				SendItems();

				//This means that i placed the PickedItem in an empty slot, so
				//the new PickedItem has id = 0
				if (m_Items[m_PickedSlot].id <= 0) {
					m_IsItemPicked = false;
					m_PickedSlot = -1;
				}

				SetItemOffsets(CreateItemOffsets());

				SendHotbarItems();

			}
		}
		m_LastButton = time;
	}

	if (Input::getScrollWheel() == -1) {
		if (m_IsInventoryOpen) return;

		m_SelectedHotbarSlot += 1;
		if (m_SelectedHotbarSlot > 8) m_SelectedHotbarSlot = 0;

		s_SelectedHotbarItem = &m_Items[m_SelectedHotbarSlot + 27];
	}
	else if (Input::getScrollWheel() == 1) {
		if (m_IsInventoryOpen) return;

		m_SelectedHotbarSlot -= 1;
		if (m_SelectedHotbarSlot < 0) m_SelectedHotbarSlot = 8;

		s_SelectedHotbarItem = &m_Items[m_SelectedHotbarSlot + 27];
	}

	if (Input::isKeyPressed(Key::D1))	   ChangeSelectedHotbarSlot(SLOT0);
	else if (Input::isKeyPressed(Key::D2)) ChangeSelectedHotbarSlot(SLOT1);
	else if (Input::isKeyPressed(Key::D3)) ChangeSelectedHotbarSlot(SLOT2);
	else if (Input::isKeyPressed(Key::D4)) ChangeSelectedHotbarSlot(SLOT3);
	else if (Input::isKeyPressed(Key::D5)) ChangeSelectedHotbarSlot(SLOT4);
	else if (Input::isKeyPressed(Key::D6)) ChangeSelectedHotbarSlot(SLOT5);
	else if (Input::isKeyPressed(Key::D7)) ChangeSelectedHotbarSlot(SLOT6);
	else if (Input::isKeyPressed(Key::D8)) ChangeSelectedHotbarSlot(SLOT7);
	else if (Input::isKeyPressed(Key::D9)) ChangeSelectedHotbarSlot(SLOT8);

}

bool Inventory::IsInSelectedArea(const glm::vec2& bottomLeft, const glm::vec2& topRight) {
	glm::vec2 mousePos = Input::getMousePosition();

	glm::vec2 scaledBottomLeft = bottomLeft * m_Scale;
	glm::vec2 scaledTopRight = topRight * m_Scale;

	if ((mousePos.x >= GetHorizontalTranslation() + scaledBottomLeft.x && mousePos.x <= GetHorizontalTranslation() + scaledTopRight.x) &&
		(mousePos.y >= m_WindowSize.y - (GetVerticalTranslation() + scaledTopRight.y) && mousePos.y <= m_WindowSize.y - (GetVerticalTranslation() + scaledBottomLeft.y))) {

		return true;
	}
	return false;
}

glm::vec3 Inventory::GetSlotTranslationVector(const int slotIndex) const
{
	const glm::vec2 slotInnerOffset = glm::vec2(7.0f, 65.0f);

	const glm::vec2 offsetToSlotCenter = glm::vec2(m_SlotSize / 2);

	int ySlotIndex = slotIndex / m_Columns;
	int xSlotIndex = slotIndex % m_Columns;

	if (ySlotIndex >= 3)
		return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(xSlotIndex * m_SlotSize, (-ySlotIndex * m_SlotSize) - 4.0f, 0)) * glm::vec3(m_Scale, 1);

	return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(xSlotIndex * m_SlotSize, -ySlotIndex * m_SlotSize, 0)) * glm::vec3(m_Scale, 1);

}

std::vector<glm::vec3> Inventory::CreateItemOffsets() {

	std::vector<glm::vec3> offsets{};

	for (auto& item : m_Items) {
		if (item.id <= 0) continue;

		if (item.slot == m_PickedSlot) {
			glm::vec2 mousePos = Input::getMousePosition();
			offsets.push_back(glm::vec3(mousePos.x, m_WindowSize.y - mousePos.y, 0));
		}
		else {
			offsets.push_back(GetSlotTranslationVector(item.slot));
		}
	}

	return offsets;
}

void Inventory::SetItemOffsets(const std::vector<glm::vec3>& offsets)
{
	m_InventoryItems.SetTransform(offsets);
}

void Inventory::SwapItems(const int i, const int j) {
	InventoryItem previousItem = m_Items[i];

	m_Items[i] = m_Items[j];
	m_Items[i].slot = i;

	m_Items[j] = previousItem;
	m_Items[j].slot = j;
}

void Inventory::SendItems() {
	std::vector<InventoryItem> toSend{};
	for (auto& item : m_Items)
	{
		if (item.id > 0) toSend.push_back(item);
	}
	m_InventoryItems.SetItems(toSend);
}

void Inventory::SendHotbarItems() {
	HotBar.SetItems(std::vector<InventoryItem>(m_Items.begin() + 27, m_Items.begin() + 27 + 9));
}

void Inventory::ChangeSelectedHotbarSlot(const int hotbarSlotIndex) {
	std::chrono::milliseconds time = Utils::GetMs();
	if ((time - m_LastButton).count() > 20) {

		if (m_IsInventoryOpen) {
			SwapItems(m_HoveredSlot, hotbarSlotIndex + 27);

			SendItems();
			SetItemOffsets(CreateItemOffsets());

			SendHotbarItems();
		}
		else {
			m_SelectedHotbarSlot = hotbarSlotIndex;
			s_SelectedHotbarItem = &m_Items[m_SelectedHotbarSlot + 27];
		}
	}
	m_LastButton = time;
}

void Inventory::SetVAO() {
	VAO.Bind();
	m_VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(GUIVertex), (void*)0);
	VAO.LinkAttrib(m_VBO, 1, 2, GL_FLOAT, sizeof(GUIVertex), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	m_VBO.Unbind();
	EBO.Unbind();
};

void Inventory::Transform() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GetTranslationVector());
	model = glm::scale(model, glm::vec3(m_Scale, 1.0));

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetVec2("slotOffset", m_SlotOffset);
	m_Shader.SetBool("isInventoryOpen", m_IsInventoryOpen);
}

void Inventory::Draw() {
	HotBar.Draw();
	if (!m_IsInventoryOpen) return;

	m_Shader.Activate();

	VAO.Bind();
	Transform();
	static_cast<TextureAtlas&>(TextureManager::GetTexture("inventory.png")).BindAtlas("GUIProgram", "tex1", 3);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	m_InventoryItems.Draw();

};

//! Event Callbacks
void Inventory::OnWindowResize(const WindowResizeEvent& e) {
	LOG_WARN("Inventory: {0}, {1}", e.m_Width, e.m_Height);
	m_WindowSize = Window::GetInstance().getWindowSize();
	Transform();
	m_InventoryItems.UpdateTransform(CreateItemOffsets());
}

void Inventory::OnBlockPlaced(const BlockPlacedEvent& e)
{
	s_SelectedHotbarItem->count -= 1;

	if (s_SelectedHotbarItem->count <= 0) {
		*s_SelectedHotbarItem = InventoryItem{ 0, 0, s_SelectedHotbarItem->slot }; //EMPTY SLOT

		SendItems();
		SetItemOffsets(CreateItemOffsets());

		SendHotbarItems();
	}
}
