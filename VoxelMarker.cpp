#include "VoxelMarker.h"

VoxelMarker::VoxelMarker(VoxelHandler* handler)
	: m_Handler(handler)
{
	GenerateMesh();
	SetVAO();
};


void VoxelMarker::GenerateMesh()
{
	m_Vertices.clear();
	m_Indices.clear();
	m_CountIndices = 0;

	for (int i = 0; i < 6; i++)
	{
		GenerateFace((BlockFace)i);
	}

}

void VoxelMarker::GenerateFace(BlockFace face)
{
	std::vector<glm::vec3> rawVertices = rawVertexData.at(face);
	for (int i = 0; i < rawVertices.size(); i++)
	{
		m_Vertices.push_back(VoxelMarkerVertex{ rawVertices[i] });
	}

	m_Indices.push_back(m_CountIndices);
	m_Indices.push_back(m_CountIndices + 1);

	m_Indices.push_back(m_CountIndices + 1);
	m_Indices.push_back(m_CountIndices + 2);

	m_Indices.push_back(m_CountIndices + 2);
	m_Indices.push_back(m_CountIndices + 3);

	m_Indices.push_back(m_CountIndices + 3);
	m_Indices.push_back(m_CountIndices);

	m_CountIndices += 4;
}

void VoxelMarker::SetVAO() {
	VAO.Bind();
	VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(VoxelMarkerVertex), (void*)0);
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}
void VoxelMarker::Draw() {
	VAO.Bind();

	glDrawElements(GL_LINES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void VoxelMarker::render(glm::vec3 position) {
	if (m_Handler->GetVoxelId() <= 0) return;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);

	model = glm::translate(model, floor(m_Handler->GetVoxelWorldPos()));
	scale = glm::scale(scale, glm::vec3(1.003, 1.003, 1.003));

	Shader shader = ShaderManager::GetShader("VoxelMarkerProgram");

	shader.Activate();
	shader.SetMat4("model", model);
	shader.SetMat4("scale", scale);

	Draw();
}