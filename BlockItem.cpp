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


	//for (auto& item : m_Items)
	//{
	//	BlockType type = (BlockType)item.id;
	//	int slot = item.slot;
	//	GenerateBlock(type, slot);
	//}


	GenerateBlock(DIRT, 0);

	SetVAO();
	Transform();

}
void BlockItem::GenerateBlock(BlockType type, const int slot) {

	GenerateFace(type, FRONT_FACE, slot);
	GenerateFace(type, RIGHT_FACE, slot);
	GenerateFace(type, TOP_FACE, slot);
	//GenerateFace(type, BACK_FACE, slot);
	//GenerateFace(type, LEFT_FACE, slot);
	//GenerateFace(type, BOTTOM_FACE, slot);
}

void BlockItem::GenerateFace(BlockType type, BlockFace face, const int slot)
{
	const auto [x, y] = UVs.at(type).at(face);
	std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(x, y);

	std::vector<glm::vec3> vertices = rawVertexData.at(face);

	/*for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = TransformVertex(vertices[i], slot);
	}*/

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
	m_VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);
	instanceBuffer.SetVertices(instanceVertices);


	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(BlockItemVertex), (void*)0);
	//VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(BlockItemVertex), (void*)(3 * sizeof(float)));

	VAO.LinkAttrib(instanceBuffer, 1, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)0);
	VAO.LinkAttrib(instanceBuffer, 2, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(sizeof(glm::vec4)));

	VAO.LinkAttrib(instanceBuffer, 3, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(2 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 4, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4)));


	VAO.LinkAttrib(instanceBuffer, 5, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(4 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 6, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(5 * sizeof(glm::vec4)));


	//VAO.LinkAttrib(instanceBuffer, 2, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)0);
	//VAO.LinkAttrib(instanceBuffer, 3, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(sizeof(glm::vec4)));
	//VAO.LinkAttrib(instanceBuffer, 4, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(2 * sizeof(glm::vec4)));
	//VAO.LinkAttrib(instanceBuffer, 5, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(3 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 7, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(6 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 8, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(6 * sizeof(glm::vec4) + sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 9, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(6 * sizeof(glm::vec4) + 2 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceBuffer, 10, 4, GL_FLOAT, sizeof(InstanceItemVertex), (void*)(6 * sizeof(glm::vec4) + 3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);
	glVertexAttribDivisor(10, 1);

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	m_VBO.Unbind();
	EBO.Unbind();
	instanceBuffer.Unbind();
}

void BlockItem::Draw()
{
	m_Shader.Activate();
	LOG_WARN("DRAWING");
	VAO.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0, m_Items.size());
}

void BlockItem::Transform() {
	float size = (m_Scale.x * 16.0f) * 0.6;
	auto m_WindowSize = Window::GetInstance().getWindowSize();

	float zOffset = 1.8f * size;

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.0f, 2.5f * zOffset);


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));
	model = glm::translate(model, glm::vec3(-0.5));


	m_Shader.Activate();
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetMat4("model", model);

}


void BlockItem::SetItems(const std::vector<InventoryItem>& items) {
	m_Items = items;
	GenerateMesh();
}

void BlockItem::SetTransformAndScale(const std::vector<Temp>& instanceTransform, const glm::vec2& scale) {
	//this->m_Position = position;
	this->m_Scale = scale;

	float size = (m_Scale.x * 16.0f) * 0.6;
	glm::mat4 view = glm::mat4(1.0f);
	float zOffset = 1.8f * size;
	//CHANGE
	for (auto& temp : instanceTransform) {
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(temp.transform.x, temp.transform.y, -zOffset));


		instanceVertices.push_back(InstanceItemVertex(temp.UV, temp.UV1, temp.UV, temp.UV1, temp.UV, temp.UV1, view));
	}


}

void BlockItem::UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale)
{
	//SetTransformAndScale(position, scale);
	//GenerateMesh();
}

