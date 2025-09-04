#include "BlockItem.h"
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>


#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"

BlockItem::BlockItem(BlockType type)
{
	LOG_INFO("BlockItem Created");
	m_Shader = ShaderManager::GetShader("Cube2DProgram");
	this->type = type;
	LOG_WARN("QUI");
	GenerateMesh();
}

//BlockItem::BlockItem(BlockType type)
//	: BlockItem(type)
//{
//	LOG_ERROR("QUI");
//	this->type = type;
//	GenerateMesh();
//}


void BlockItem::GenerateMesh()
{
	m_Vertices.clear();
	m_Indices.clear();

	GenerateFace(type, FRONT_FACE);
	GenerateFace(type, RIGHT_FACE);
	GenerateFace(type, TOP_FACE);
	GenerateFace(type, BACK_FACE);
	GenerateFace(type, LEFT_FACE);
	GenerateFace(type, BOTTOM_FACE);

	SetVAO();
	Transform();

}

void BlockItem::GenerateFace(BlockType type, BlockFace face)
{
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
	UpdateTransform();
	//Transform();

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void BlockItem::Transform() {
	float size = (scale.x * 16.0f) * 0.6;
	auto m_WindowSize = Window::GetInstance().getWindowSize();

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));
	model = glm::translate(model, glm::vec3(-0.5)); // To rotate around the middle, verices have to be between -0.5 and 0.5

	view = glm::mat4(1.0f);
	float zOffset = 1.8f * size;
	view = glm::translate(view, glm::vec3(position.x, position.y, -zOffset));

	proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.0f, 2.5f * zOffset);
	UpdateTransform();

}

void BlockItem::UpdateTransform()
{
	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("view", view);
	m_Shader.SetMat4("proj", proj);
}

void BlockItem::UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale)
{
	this->position = position;
	this->scale = scale;
	Transform();
}

