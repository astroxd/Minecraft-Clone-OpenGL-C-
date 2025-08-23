#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	unsigned int compressed;

};

class VBO {
public:
	GLuint ID;
	VBO() {};
	VBO(std::vector<Vertex>& vertices);

	void setVertices(std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif;
