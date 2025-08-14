#include "VoxelMarker.h"

VoxelMarker::VoxelMarker(VoxelHandler* handler)
	: m_Handler(handler)
{
	GenerateMesh();
	setVAO();
};

void VoxelMarker::setVAO() {
	VAO.Bind();
	VBO.setVertices(vertices);
	EBO.setIndices(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}
void VoxelMarker::GenerateMesh()
{
	vertices.clear();
	indices.clear();
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
		vertices.push_back(Vertex{ rawVertices[i] });
	}

	indices.push_back(m_CountIndices);
	indices.push_back(m_CountIndices + 1);

	indices.push_back(m_CountIndices + 1);
	indices.push_back(m_CountIndices + 2);

	indices.push_back(m_CountIndices + 2);
	indices.push_back(m_CountIndices + 3);

	indices.push_back(m_CountIndices + 3);
	indices.push_back(m_CountIndices);

	m_CountIndices += 4;
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

	VAO.Bind();

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}