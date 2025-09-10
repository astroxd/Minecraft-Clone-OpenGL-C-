#include "Inventory.h"
#include "Log.h"
#include "Utils.h"


Inventory::Inventory() {
	LOG_INFO("Inventory Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");


	Items.reserve(36);
	for (int i = 0; i < 36; i++)
	{
		Items.emplace_back(InventoryItem{ 0, 0, i });
	}
	for (auto& item : InventoryItems)
	{
		Items[item.slot] = item;
	}

	std::vector<InventoryItem> toSend{};
	for (auto& item : Items)
	{
		if (item.id > 0) toSend.push_back(item);
	}
	m_InventoryItems.SetItems(toSend);
	SetItemOffsets(CreateItemOffsets());

	GenerateMesh();

}

void Inventory::GenerateMesh() {
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

void Inventory::Draw() {
	if (!m_IsInventoryOpen) return;

	m_Shader.Activate();

	VAO.Bind();
	Transform();
	static_cast<TextureAtlas&>(TextureManager::GetTexture("inventory.png")).BindAtlas("GUIProgram", "tex1", 3);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	m_InventoryItems.Draw();

};

void Inventory::Update() {
	glm::vec2 mousePos = Input::getMousePosition();

	m_Shader.Activate();

	UpdateWindowSize();
	HandleInput();

	if (!m_IsInventoryOpen) return;

	if (IsInSelectedArea(glm::vec2(7.0f, 30.0f), glm::vec2(169.0f, 83.0f)) ||
		IsInSelectedArea(glm::vec2(7.0f, 7.0f), glm::vec2(169.0f, 25.0f)))
	{
		float localY = mousePos.y - (GetVerticalTranslation() + GetScaledSize(83.0f));
		float localX = mousePos.x - (GetHorizontalTranslation() + GetScaledSize(7.0f));

		int ySlotIndex = std::min(int(localY / GetScaledSize(m_SlotSize)), m_Rows - 1);
		int xSlotIndex = std::min(int((localX / GetScaledSize(m_SlotSize))), m_Columns - 1);

		int slot = xSlotIndex + ySlotIndex * 8 + ySlotIndex;

		if (ySlotIndex >= 3) {
			m_SlotOffset = glm::vec2(xSlotIndex, -ySlotIndex) * m_SlotSize - glm::vec2(0.0f, 4.0f);
		}
		else {
			m_SlotOffset = glm::vec2(xSlotIndex, -ySlotIndex) * m_SlotSize;
		}

		m_Shader.SetVec2("slotOffset", m_SlotOffset);
		m_Shader.SetBool("highlightSlot", true);


		if (Input::isMouseButtonPressed(Mouse::Button0)) {
			std::chrono::milliseconds time = Utils::GetMs();
			if ((time - m_LastButton).count() > 20) {
				//picked = !picked;

				if (!picked) {
					if (Items[slot].id > 0) {
						slotPicked = slot;
						picked = true;
						LOG_WARN("PICEKD");
						LOG_WARN("{0}, {1}, {2}", slot, slotPicked, picked);
					}
				}
				else {
					LOG_WARN("TRUE");
					if (Items[slot].id <= 0) {
						Items[slot] = Items[slotPicked];
						Items[slot].slot = slot;
						Items[slotPicked] = InventoryItem{ 0,0,slotPicked };
						std::vector<InventoryItem> toSend{};
						for (auto& item : Items)
						{
							if (item.id > 0) toSend.push_back(item);
						}
						m_InventoryItems.SetItems(toSend);
						picked = false;
						slotPicked = -1;
						m_InventoryItems.SetTransform(CreateItemOffsets());
					}
				}
			}
			m_LastButton = time;

		}





		/*if (!picked) {
			if (Input::isMouseButtonPressed(Mouse::Button0)) {
				std::chrono::milliseconds time = Utils::GetMs();
				if ((time - m_LastButton).count() > 20) {
					if (Items[slot].id > 0) {
						picked = true;
						slotPicked = slot;
						LOG_WARN("PICEKD");
					}
				}
				m_LastButton = time;
			}
		}
		else {
			if (Input::isMouseButtonPressed(Mouse::Button0)) {
				std::chrono::milliseconds time = Utils::GetMs();
				if ((time - m_LastButton).count() > 20) {
					if (Items[slot].id <= 0) {
						Items[slot] = Items[slotPicked];
						Items[slotPicked] = InventoryItem{ 0,0,slotPicked };
						picked = false;
					}
				}
				m_LastButton = time;
			}
		}*/


	}
	else {
		m_Shader.SetBool("highlightSlot", false);
	}

	//If Item is Picked update its position every frame
	if (picked) {
		m_InventoryItems.UpdateTransform(CreateItemOffsets());
	}


}

void Inventory::UpdateWindowSize() {
	if (Window::GetInstance().getWindowSize() != m_WindowSize) {
		m_WindowSize = Window::GetInstance().getWindowSize();
		Transform();
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

			if (!m_IsInventoryOpen) m_Shader.SetBool("highlightSlot", false);
		}
		m_LastButton = time;
	}
	else if (Input::isKeyPressed(Key::Escape)) {
		m_IsInventoryOpen = false;
		m_Shader.Activate();
		m_Shader.SetBool("isInventoryOpen", m_IsInventoryOpen);
		m_Shader.SetBool("highlightSlot", false);
	}

	/*if (Input::isMouseButtonPressed(Mouse::Button0)) {
		picked = true;
		LOG_WARN("PICEKD");
	}*/




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

glm::vec3 Inventory::GetTranslationVector() const {
	return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0);
}

glm::vec3 Inventory::GetSlotTranslationVector(int slotIndex) const
{

	//if (Items[slotIndex].id <= 0) return glm::vec3(0);

	const glm::vec2 slotInnerOffset = glm::vec2(7.0f, 65.0f);

	const glm::vec2 offsetToSlotCenter = glm::vec2(m_SlotSize / 2);

	int ySlotIndex = slotIndex / m_Columns;
	int xSlotIndex = slotIndex % m_Columns;

	if (ySlotIndex >= 3)
		return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(xSlotIndex * m_SlotSize, (-ySlotIndex * m_SlotSize) - 4.0f, 0)) * glm::vec3(m_Scale, 1);

	return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(xSlotIndex * m_SlotSize, -ySlotIndex * m_SlotSize, 0)) * glm::vec3(m_Scale, 1);

}

std::vector<glm::vec3> Inventory::CreateItemOffsets() {

	std::vector<glm::vec3> offsets = {};

	for (auto& item : Items) {
		if (item.id <= 0) continue;

		if (item.slot == slotPicked) {
			glm::vec2 mousePos = Input::getMousePosition();
			offsets.push_back(glm::vec3(mousePos.x, m_WindowSize.y - mousePos.y, 0));
			LOG_WARN("SLOTPICKED");
		}
		else {
			offsets.push_back(GetSlotTranslationVector(item.slot));
			LOG_WARN("{0}, {1}", item.slot, glm::to_string(GetSlotTranslationVector(item.slot)));
		}
	}

	return offsets;
}

void Inventory::SetItemOffsets(const std::vector<glm::vec3>& offsets)
{
	m_InventoryItems.SetTransform(offsets);
}

