#include "Hotbar.h"
#include "Utils.h"

Hotbar::Hotbar() {
	LOG_INFO("HotBar Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");
	GenerateMesh();
}

void Hotbar::GenerateMesh() {
	int m_WidthImg = 256;
	int m_HeightImg = 256;
	int countIndices = 0;

	std::vector<glm::vec2> FaceUV = static_cast<TextureAtlas&>(TextureManager::GetTexture("widget.png")).GetUV(m_xOffset, m_yOffset, m_HotBarWidth, m_HotBarHeight);

	//TODO to be implemented inside inventory
	m_Vertices.push_back(GUIVertex{ glm::vec3(3,3, 0),  glm::vec2(0, 0) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(3,19, 0) , glm::vec2(0,16.0f / m_HeightImg) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(19,19, 0) , glm::vec2(16.0f / m_WidthImg, 16.0f / m_HeightImg) });
	m_Vertices.push_back(GUIVertex{ glm::vec3(19 ,3, 0) , glm::vec2(16.0f / m_WidthImg, 0) });

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	countIndices += 4;

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

void Hotbar::Transform() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GetTranslationVector());
	model = glm::scale(model, glm::vec3(m_Scale, 1.0));

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetBool("show", true);
}

void Hotbar::SetVAO() {
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

void Hotbar::Draw() {
	m_Shader.Activate();

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	block1.Draw();
	block2.Draw();

};

void Hotbar::Update() {
	glm::vec2 mousePos = Input::getMousePosition();

	m_Shader.Activate();

	UpdateWindowSize();

	//TODO to be implemented inside inventory
	if (mousePos.x >= GetHorizontalTranslation() && mousePos.x <= GetHorizontalTranslation() + GetScaledWidth()) {

		if (mousePos.y >= m_WindowSize.y - (GetVerticalTranslation() + GetScaledHeight()) && mousePos.y <= m_WindowSize.y - GetVerticalTranslation()) {
			m_Shader.SetBool("show", true);

			int slotIndex = std::min(int(((mousePos.x - GetHorizontalTranslation()) / 40.0f)), 8);
			m_Shader.SetFloat("offset", slotIndex * 20.0f);

		}
		else {
			m_Shader.SetBool("show", false);
		}
	}
	else {
		m_Shader.SetBool("show", false);
	}

	HandleInput();
}

void Hotbar::UpdateWindowSize() {
	if (Window::GetInstance().getWindowSize() != m_WindowSize) {
		m_WindowSize = Window::GetInstance().getWindowSize();
		Transform();
	}
}

glm::vec3 Hotbar::GetTranslationVector() const {
	return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0);
}

void Hotbar::HandleInput() {
	if (Input::getScrollWheel() == -1) {
		m_Offset += m_SlotSize;

		if (m_Offset > m_SlotSize * 8) m_Offset = 0.0f;

		m_Shader.Activate();
		m_Shader.SetFloat("offset", m_Offset);
	}

	if (Input::getScrollWheel() == 1) {
		m_Offset -= m_SlotSize;

		if (m_Offset < 0.0f) m_Offset = m_SlotSize * 8;

		m_Shader.Activate();
		m_Shader.SetFloat("offset", m_Offset);
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

void Hotbar::ChangeSelectedSlot(int slotIndex) {
	std::chrono::milliseconds time = Utils::GetMs();
	if ((time - m_LastButton).count() > 20) {

		m_Offset = m_SlotSize * slotIndex;
		m_Shader.Activate();
		m_Shader.SetFloat("offset", m_Offset);
	}
	m_LastButton = time;
}