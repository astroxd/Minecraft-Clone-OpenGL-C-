#include "Mesh.h"


Mesh::Mesh() {
	//std::cout << "Mesh created" << std::endl;
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
	// Generates Vertex Buffer Object and links it to vertices
	//std::cout << "VBO: " << vertices.size() << std::endl;
	VBO.setVertices(vertices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 1, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 1, GL_FLOAT, sizeof(Vertex), (void*)(4 * sizeof(float)));
	//VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
}


void Mesh::Draw(Shader& shader) {
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();

	// Draw the actual mesh
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}