#include "BlockItem.h"
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>


#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"

BlockItem::BlockItem()
{
	LOG_INFO("BlockItem Created");
	m_Shader = ShaderManager::GetShader("Cube2DProgram");
}

void BlockItem::GenerateMesh()
{
	m_Vertices.clear();
	m_Indices.clear();
	m_CountIndices = 0;


	for (auto& item : m_Items)
	{
		BlockType type = (BlockType)item.id;
		int slot = item.slot;
		GenerateBlock(type, slot);
	}

	SetVAO();
	Transform();

}
void BlockItem::GenerateBlock(BlockType type, const int slot) {

	GenerateFace(type, FRONT_FACE, slot);
	GenerateFace(type, RIGHT_FACE, slot);
	GenerateFace(type, TOP_FACE, slot);
	GenerateFace(type, BACK_FACE, slot);
	GenerateFace(type, LEFT_FACE, slot);
	GenerateFace(type, BOTTOM_FACE, slot);
}

void BlockItem::GenerateFace(BlockType type, BlockFace face, const int slot)
{
	const auto [x, y] = UVs.at(type).at(face);
	std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(x, y);

	std::vector<glm::vec3> vertices = rawVertexData.at(face);

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = TransformVertex(vertices[i], slot);
	}

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

glm::vec3 BlockItem::TransformVertex(const glm::vec3& vertex, const int slot) {
	float size = (m_Scale.x * 16.0f) * 0.6;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));
	model = glm::translate(model, glm::vec3(-0.5)); // To rotate around the middle, verices have to be between -0.5 and 0.5

	glm::mat4 view = glm::mat4(1.0f);
	float zOffset = 1.8f * size;

	glm::vec3 slotPosition = m_Position + glm::vec3(slot * 18.0f, 0, 0) * glm::vec3(m_Scale, 1);

	view = glm::translate(view, glm::vec3(slotPosition.x, slotPosition.y, -zOffset));

	return glm::vec3(view * model * glm::vec4(vertex, 1.0f));
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

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void BlockItem::Transform() {
	float size = (m_Scale.x * 16.0f) * 0.6;
	auto m_WindowSize = Window::GetInstance().getWindowSize();

	float zOffset = 1.8f * size;

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.0f, 2.5f * zOffset);

	m_Shader.Activate();
	m_Shader.SetMat4("proj", proj);

}


void BlockItem::SetItems(const std::vector<InventoryItem>& items) {
	m_Items = items;
	GenerateMesh();
}

void BlockItem::SetTransformAndScale(const glm::vec3& position, const glm::vec2& scale) {
	this->m_Position = position;
	this->m_Scale = scale;
}

void BlockItem::UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale)
{
	SetTransformAndScale(position, scale);
	GenerateMesh();
}

