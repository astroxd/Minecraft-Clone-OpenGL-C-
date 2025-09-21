#include "Hotbar.h"
#include "Utils.h"

Hotbar::Hotbar()
	: m_WindowResizeHandler([this](const WindowResizeEvent& e) { OnWindowResize(e); })
{
	LOG_INFO("HotBar Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");

	Events::Subscribe(m_WindowResizeHandler);

	GenerateMesh();
}

void Hotbar::GenerateMesh() {
	m_Vertices.clear();
	m_Indices.clear();
	int countIndices = 0;

	//! SLOT SELECTOR
	slotSelector.TransformVertices(glm::vec3(-1, -1, 0));
	for (int i = 0; i < 4; i++)
	{
		m_Vertices.push_back(GUIVertex{ slotSelector.vertices[i],  slotSelector.UV[i] });
	}

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	countIndices += 4;
	////////////////////////////////////////////

	//! HOTBAR
	std::vector<glm::vec2> FaceUV = static_cast<TextureAtlas&>(TextureManager::GetTexture("widget.png")).GetUV(m_xOffset, m_yOffset, m_HotBarWidth, m_HotBarHeight);

	m_Vertices.push_back(GUIVertex{ glm::vec3(0, 0, 0),  FaceUV[0] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(0, m_HotBarHeight, 0) , FaceUV[1] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(m_HotBarWidth,m_HotBarHeight, 0) , FaceUV[2] });
	m_Vertices.push_back(GUIVertex{ glm::vec3(m_HotBarWidth ,0, 0) , FaceUV[3] });

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

void Hotbar::Update() {
	HandleInput();
}

glm::vec3 Hotbar::GetTranslationVector() const {
	return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0);
}

glm::vec3 Hotbar::GetSlotTranslationVector(int slotIndex) const
{
	const glm::vec2 slotInnerOffset = glm::vec2(3);
	const float slotInnerWidth = 16.0f;
	slotIndex -= 27; //slotIndex ranges from 0-36, need to have them between 0-9

	glm::vec2 offsetToSlotCenter = glm::vec2(slotInnerWidth / 2);
	return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(slotIndex * 20.0f, 0, 0)) * glm::vec3(m_Scale, 1);

}

void Hotbar::HandleInput() {
	if (m_IsInventoryOpen) return;

	if (Input::getScrollWheel() == -1) {
		m_SlotOffset += m_SlotSize;

		if (m_SlotOffset > m_SlotSize * 8) m_SlotOffset = 0.0f;

		m_Shader.Activate();
		m_Shader.SetVec2("slotOffset", glm::vec2(m_SlotOffset, 0.0f));
	}
	else if (Input::getScrollWheel() == 1) {
		m_SlotOffset -= m_SlotSize;

		if (m_SlotOffset < 0.0f) m_SlotOffset = m_SlotSize * 8;

		m_Shader.Activate();
		m_Shader.SetVec2("slotOffset", glm::vec2(m_SlotOffset, 0.0f));
	}

	if (Input::isKeyPressed(Key::D1)) ChangeSelectedSlot(SLOT0);
	else if (Input::isKeyPressed(Key::D2)) ChangeSelectedSlot(SLOT1);
	else if (Input::isKeyPressed(Key::D3)) ChangeSelectedSlot(SLOT2);
	else if (Input::isKeyPressed(Key::D4)) ChangeSelectedSlot(SLOT3);
	else if (Input::isKeyPressed(Key::D5)) ChangeSelectedSlot(SLOT4);
	else if (Input::isKeyPressed(Key::D6)) ChangeSelectedSlot(SLOT5);
	else if (Input::isKeyPressed(Key::D7)) ChangeSelectedSlot(SLOT6);
	else if (Input::isKeyPressed(Key::D8)) ChangeSelectedSlot(SLOT7);
	else if (Input::isKeyPressed(Key::D9)) ChangeSelectedSlot(SLOT8);
}

void Hotbar::ChangeSelectedSlot(const int slotIndex) {
	std::chrono::milliseconds time = Utils::GetMs();
	if ((time - m_LastButton).count() > 20) {

		m_SlotOffset = m_SlotSize * slotIndex;
		m_Shader.Activate();
		m_Shader.SetVec2("slotOffset", glm::vec2(m_SlotOffset, 0.0f));
	}
	m_LastButton = time;
}

void Hotbar::SetItems(const std::vector<InventoryItem>& items) {
	m_Items = items;

	SendItems();
	m_HotBarItems.SetTransform(CreateItemOffsets());
}

void Hotbar::SendItems() {
	std::vector<InventoryItem> toSend{};
	for (auto& item : m_Items)
	{
		if (item.id > 0) toSend.push_back(item);
	}
	m_HotBarItems.SetItems(toSend);
}

std::vector<glm::vec3> Hotbar::CreateItemOffsets() {

	std::vector<glm::vec3> offsets{};

	for (auto& item : m_Items) {
		if (item.id <= 0) continue;
		offsets.push_back(GetSlotTranslationVector(item.slot));
	}

	return offsets;
}

void Hotbar::SetInventoryOpen(bool inventoryOpen) {
	m_IsInventoryOpen = inventoryOpen;
}

void Hotbar::SetVAO() {
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
}

void Hotbar::Transform() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GetTranslationVector());
	model = glm::scale(model, glm::vec3(m_Scale, 1.0));

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetVec2("slotOffset", glm::vec2(m_SlotOffset, 0.0f));
	m_Shader.SetBool("isInventoryOpen", false);
}

void Hotbar::Draw() {
	m_Shader.Activate();

	VAO.Bind();
	Transform();
	m_Shader.SetBool("isInventoryOpen", false);
	static_cast<TextureAtlas&>(TextureManager::GetTexture("widget.png")).BindAtlas("GUIProgram", "tex1", 1);

	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	m_HotBarItems.Draw();

}

//! Event Callbacks
void Hotbar::OnWindowResize(const WindowResizeEvent& e) {
	m_WindowSize = Window::GetInstance().getWindowSize();
	Transform();
	m_HotBarItems.UpdateTransform(CreateItemOffsets());
}
