#include "BlockItem.h"
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>

#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"

BlockItem::BlockItem(glm::vec2 scale)
	: m_Scale(scale)
{
	LOG_INFO("BlockItem Created");
	m_Shader = ShaderManager::GetShader("Cube2DProgram");
}

void BlockItem::GenerateMesh()
{
	m_Vertices.clear();
	m_Indices.clear();
	m_CountIndices = 0;

	GenerateBlock();

	SetVAO();
	Transform();

}

void BlockItem::GenerateBlock() {

	GenerateFace(TOP_FACE);
	GenerateFace(BOTTOM_FACE);
	GenerateFace(RIGHT_FACE);
	GenerateFace(LEFT_FACE);
	GenerateFace(FRONT_FACE);
	GenerateFace(BACK_FACE);
}

void BlockItem::GenerateFace(BlockFace face)
{

	std::vector<glm::vec3> vertices = rawVertexData.at(face);

	m_Vertices.push_back(BlockItemVertex{ vertices[0] });
	m_Vertices.push_back(BlockItemVertex{ vertices[1] });
	m_Vertices.push_back(BlockItemVertex{ vertices[2] });
	m_Vertices.push_back(BlockItemVertex{ vertices[3] });

	m_Indices.push_back(m_CountIndices);
	m_Indices.push_back(m_CountIndices + 3);
	m_Indices.push_back(m_CountIndices + 2);

	m_Indices.push_back(m_CountIndices);
	m_Indices.push_back(m_CountIndices + 2);
	m_Indices.push_back(m_CountIndices + 1);

	m_CountIndices += 4;
}

void BlockItem::SetItems(const std::vector<InventoryItem>& items) {
	m_Items = items;
}

void BlockItem::SetTransform(const std::vector<glm::vec3>& instancePositions) {
	m_InstanceVertices.clear();
	for (int i = 0; i < instancePositions.size(); i++) {
		glm::mat4 view = GenerateInstanceView(instancePositions[i]);
		BlockUVs BlockUVs = GenerateBlockTexture(i);

		m_InstanceVertices.push_back(InstanceItemVertex(BlockUVs, view));
	}

	GenerateMesh();
}

void BlockItem::UpdateTransform(const std::vector<glm::vec3>& instancePositions)
{

	for (int i = 0; i < instancePositions.size(); i++) {
		glm::mat4 view = GenerateInstanceView(instancePositions[i]);

		m_InstanceVertices[i].view = view;
	}

	//Needs to be called every time window change dimension
	Transform();

	SetVAO();
}

glm::mat4 BlockItem::GenerateInstanceView(const glm::vec3& position)
{
	float size = (m_Scale.x * 16.0f) * 0.6;
	glm::mat4 view = glm::mat4(1.0f);
	float zOffset = 1.8f * size;

	return view = glm::translate(view, glm::vec3(position.x, position.y, -zOffset));
}

BlockUVs BlockItem::GenerateBlockTexture(const int InventoryIndex) {
	BlockUVs BlockUVs{};

	for (int i = 0; i < 6; i++)
	{
		const auto& [x, y] = UVs.at((BlockType)m_Items[InventoryIndex].id).at((BlockFace)i);
		std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(x, y);
		memcpy(reinterpret_cast<glm::vec2*>(&BlockUVs) + i, UVs.data(), sizeof(glm::vec2));
	}
	return BlockUVs;
}

void BlockItem::Transform() {
	float size = (m_Scale.x * 16.0f) * 0.6;
	auto WindowSize = Window::GetInstance().getWindowSize();


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));
	model = glm::translate(model, glm::vec3(-0.5));

	float zOffset = 1.8f * size;
	glm::mat4 proj = glm::ortho(0.0f, WindowSize.x, 0.0f, WindowSize.y, 0.0f, 2.5f * zOffset);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
}

void BlockItem::SetVAO()
{
	VAO.Bind();
	m_VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);
	m_InstanceVBO.SetVertices(m_InstanceVertices);


	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(BlockItemVertex), (void*)0);

	VAO.LinkAttrib(m_InstanceVBO, 1, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)0);
	VAO.LinkAttrib(m_InstanceVBO, 2, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(sizeof(glm::vec4)));
	VAO.LinkAttrib(m_InstanceVBO, 3, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(2 * sizeof(glm::vec4)));

	VAO.LinkAttrib(m_InstanceVBO, 4, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4)));
	VAO.LinkAttrib(m_InstanceVBO, 5, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4) + sizeof(glm::vec4)));
	VAO.LinkAttrib(m_InstanceVBO, 6, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4) + 2 * sizeof(glm::vec4)));
	VAO.LinkAttrib(m_InstanceVBO, 7, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4) + 3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	m_VBO.Unbind();
	EBO.Unbind();
	m_InstanceVBO.Unbind();
}

void BlockItem::Draw()
{
	m_Shader.Activate();
	VAO.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0, m_Items.size());
}