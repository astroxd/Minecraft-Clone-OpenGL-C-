#include "BlockItem.h"
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>


#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"

BlockItem::BlockItem() {
	LOG_INFO("BlockItem Created");
	m_Shader = ShaderManager::GetShader("Cube2DProgram");
	GenerateMesh();
}

void BlockItem::GenerateMesh() {
	std::vector<glm::vec3> vertices = rawVertexData.at(FRONT_FACE);
	std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(14, 14);


	GenerateFace(GRASS, FRONT_FACE);
	GenerateFace(GRASS, RIGHT_FACE);
	GenerateFace(GRASS, TOP_FACE);

	GenerateFace(GRASS, BACK_FACE);
	GenerateFace(GRASS, LEFT_FACE);
	GenerateFace(GRASS, BOTTOM_FACE);



	SetVAO();
	Transform();

}

void BlockItem::GenerateFace(BlockType type, BlockFace face) {
	const auto [x, y] = UVs.at(type).at(face);
	std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(x, y);

	std::vector<glm::vec3> vertices = rawVertexData.at(face);

	m_Vertices.push_back(BlockItemVertex{ vertices[0], UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ vertices[1], UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ vertices[2], UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ vertices[3],  UVs[3] });

	m_Indices.push_back(m_CountIndices);
	m_Indices.push_back(m_CountIndices + 3);
	m_Indices.push_back(m_CountIndices + 2);

	m_Indices.push_back(m_CountIndices);
	m_Indices.push_back(m_CountIndices + 2);
	m_Indices.push_back(m_CountIndices + 1);

	m_CountIndices += 4;
}


void BlockItem::SetVAO()
{
	VAO.Bind();
	VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(BlockItemVertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(BlockItemVertex), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void BlockItem::Draw()
{
	m_Shader.Activate();

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(100, 0, 0));
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-35.0f), glm::vec3(0, 2, 0));
	model = glm::scale(model, glm::vec3(160.0f));

	//m_Shader.SetMat4("model", model);

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void BlockItem::Transform() {
	float size = 160.0f;
	auto m_WindowSize = Window::GetInstance().getWindowSize();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));


	glm::mat4 view = glm::mat4(1.0f);


	float zOffset = 1.8f * size;
	view = glm::translate(view, glm::vec3(510, 20, -zOffset));

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.0f, 2.5f * zOffset);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetMat4("view", view);
}
