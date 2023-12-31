#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	unsigned int voxelId;
	unsigned int faceId;
	glm::vec2 UV;
	int aoId;
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
