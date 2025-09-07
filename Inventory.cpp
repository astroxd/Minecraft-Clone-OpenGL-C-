#include "Inventory.h"
#include "Log.h"
#include "Utils.h"


Inventory::Inventory() {
	LOG_INFO("Inventory Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");

	m_InventoryItems.SetTransformAndScale(GetSlotTranslationVector(0), m_Scale);
	m_InventoryItems.SetItems(InventoryItems);


	GenerateMesh();

}

void Inventory::GenerateMesh() {
	int m_WidthImg = 256;
	int m_HeightImg = 256;
	int countIndices = 0;

	std::vector<glm::vec2> FaceUV = static_cast<TextureAtlas&>(TextureManager::GetTexture("inventory.png")).GetUV(m_xOffset, m_yOffset, m_InventoryWidth, m_InventoryHeight);

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



	//! Inventory
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
	m_Shader.SetBool("isInventoryOpen", isInventoryOpen);
}

void Inventory::SetVAO() {
	VAO.Bind();
	VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(GUIVertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(GUIVertex), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
};

void Inventory::Draw() {
	if (!isInventoryOpen) return;

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

	if (!isInventoryOpen) return;

	if (IsInSelectedArea(glm::vec2(7.0f, 30.0f), glm::vec2(169.0f, 83.0f)) ||
		IsInSelectedArea(glm::vec2(7.0f, 7.0f), glm::vec2(169.0f, 25.0f)))
	{
		float localY = mousePos.y - (GetVerticalTranslation() + GetScaledSize(83.0f));
		float localX = mousePos.x - (GetHorizontalTranslation() + GetScaledSize(7.0f));

		int ySlotIndex = std::min(int(localY / GetScaledSize(m_SlotSize)), m_Rows - 1);
		int xslotIndex = std::min(int((localX / GetScaledSize(m_SlotSize))), m_Columns - 1);

		int slot = xslotIndex + ySlotIndex * 8 + ySlotIndex;

		if (ySlotIndex >= 3) {
			m_SlotOffset = glm::vec2(xslotIndex, -ySlotIndex) * m_SlotSize - glm::vec2(0.0f, 4.0f);
		}
		else {
			m_SlotOffset = glm::vec2(xslotIndex, -ySlotIndex) * m_SlotSize;
		}

		m_Shader.SetVec2("slotOffset", m_SlotOffset);
		m_Shader.SetBool("highlightSlot", true);
	}
	else {
		m_Shader.SetBool("highlightSlot", false);
	}
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

void Inventory::UpdateWindowSize() {
	if (Window::GetInstance().getWindowSize() != m_WindowSize) {
		m_WindowSize = Window::GetInstance().getWindowSize();
		Transform();
		m_InventoryItems.SetTransformAndScale(GetSlotTranslationVector(0), m_Scale);
	}
}

void Inventory::HandleInput() {

	if (Input::isKeyPressed(Key::E)) {
		std::chrono::milliseconds time = Utils::GetMs();
		if ((time - m_LastButton).count() > 20) {
			isInventoryOpen = !isInventoryOpen;
			m_Shader.Activate();
			m_Shader.SetBool("isInventoryOpen", isInventoryOpen);

			if (!isInventoryOpen) m_Shader.SetBool("highlightSlot", false);
		}
		m_LastButton = time;
	}
	else if (Input::isKeyPressed(Key::Escape)) {
		isInventoryOpen = false;
		m_Shader.Activate();
		m_Shader.SetBool("isInventoryOpen", isInventoryOpen);
		m_Shader.SetBool("highlightSlot", false);
	}
}


glm::vec3 Inventory::GetTranslationVector() const {
	return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0);
}

glm::vec3 Inventory::GetSlotTranslationVector(int slotIndex) const
{
	const glm::vec2 slotInnerOffset = glm::vec2(7.0f, 65.0f);
	const float slotInnerWidth = 18.0f;

	glm::vec2 offsetToSlotCenter = glm::vec2(slotInnerWidth / 2);

	return GetTranslationVector() + (glm::vec3(slotInnerOffset + offsetToSlotCenter, 0.0) + glm::vec3(slotIndex * m_SlotSize, 0, 0)) * glm::vec3(m_Scale, 1);

}