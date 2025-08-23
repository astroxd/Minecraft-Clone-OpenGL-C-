#include "Mesh.h"
#include "Log.h"

Mesh::Mesh() {
	//LOG_INFO("Mesh created");
}

Mesh::Mesh(std::vector <Vertex>& vertices) {
	Mesh::vertices = vertices;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO.setVertices(vertices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();

}

void Mesh::setData(std::vector <Vertex>& vertices) {
	Mesh::vertices = vertices;
	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO.setVertices(vertices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(3 * sizeof(unsigned int)));
	VAO.LinkAttrib(VBO, 2, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(4 * sizeof(unsigned int)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
}

void Mesh::setVAO() {
	VAO.Bind();

	glCreateBuffers(1, &ssbo);


	glNamedBufferStorage(ssbo, vertices.size() * sizeof(Vertex), (const void*)vertices.data(), NULL);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);


	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}


void Mesh::Draw(Shader& shader) {
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	//std::cout << "Draw Mesh" << vertices.size() << " - " << indices.size() << std::endl;

	// Draw the actual mesh
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 6);
}