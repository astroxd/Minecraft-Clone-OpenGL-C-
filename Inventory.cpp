#include "Inventory.h"
#include "Log.h"


Inventory::Inventory() {
	LOG_INFO("Inventory Created");
	m_Shader = ShaderManager::GetShader("GUIProgram");
	GenerateMesh();

}

void Inventory::GenerateMesh() {
	int m_WidthImg = 256;
	int m_HeightImg = 256;
	int countIndices = 0;

	std::vector<glm::vec2> FaceUV = static_cast<TextureAtlas&>(TextureManager::GetTexture("inventory.png")).GetUV(m_xOffset, m_yOffset, m_InventoryWidth, m_InventoryHeight);

	//TODO to be implemented inside inventory
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
	//m_Shader.SetBool("show", true);
	m_Shader.SetBool("isInventoryOpen", true);
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
	m_Shader.Activate();

	VAO.Bind();
	Transform();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	//m_HotBarItems.Draw();

};

void Inventory::Update() {
	glm::vec2 mousePos = Input::getMousePosition();

	m_Shader.Activate();

	//UpdateWindowSize();

	//INSIDE INVENTORY TEXTURE
	if (mousePos.x >= GetHorizontalTranslation() && mousePos.x <= GetHorizontalTranslation() + GetScaledWidth()) {
		if (mousePos.y >= m_WindowSize.y - (GetVerticalTranslation() + GetScaledHeight()) && mousePos.y <= m_WindowSize.y - GetVerticalTranslation()) {

			//INSIDE INVENTORY SLOTS
			if (mousePos.x - GetHorizontalTranslation() - 21.0f >= 0 && mousePos.x - GetHorizontalTranslation() - 21.0f <= GetScaledWidth() - 42.0f) {
				if (mousePos.y >= m_WindowSize.y - (GetVerticalTranslation() + GetScaledHeight()) + 249.0f && mousePos.y <= m_WindowSize.y - GetVerticalTranslation() - 21.0f) {
					float localY = mousePos.y - (GetVerticalTranslation() + GetScaledHeight()) + 249.0f;
					int ySlotIndex = std::min(int(localY / 54.0f), 3);

					m_Shader.SetBool("show", true);
					int slotIndex = std::min(int(((mousePos.x - GetHorizontalTranslation() - 21.0f) / 54.0f)), 8);
					//LOG_WARN(((mousePos.x - GetHorizontalTranslation() - 21.0f)));
					LOG_WARN("{0}, {1}", slotIndex, ySlotIndex);
					if (ySlotIndex >= 3) {

						m_Shader.SetVec2("offset", glm::vec2(slotIndex * 18.0f, (-ySlotIndex) * 18.0f - 4.0f));
					}
					else {
						m_Shader.SetVec2("offset", glm::vec2(slotIndex * 18.0f, (-ySlotIndex) * 18.0f));
					}
				}
				else {
					m_Shader.SetBool("show", false);
				}
			}
			else {
				m_Shader.SetBool("show", false);
			}
		}
		else {
			m_Shader.SetBool("show", false);
		}
	}
	else {
		m_Shader.SetBool("show", false);
	}
}




glm::vec3 Inventory::GetTranslationVector() const {
	return glm::vec3(GetHorizontalTranslation(), GetVerticalTranslation(), 0.0);
}