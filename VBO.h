#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<vector>

template <typename VertexType>
class VBO {
public:
	GLuint ID;

	VBO() {
		glGenBuffers(1, &ID);
	}
	~VBO() {
		Delete();
	}

	void SetVertices(const std::vector<VertexType>& vertices) {
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() {
		glDeleteBuffers(1, &ID);
	}
};

#endif;
