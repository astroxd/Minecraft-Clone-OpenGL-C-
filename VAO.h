#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <iostream>

#include "VBO.h"

template <typename T>
class VAO {
public:
	GLuint ID;

	VAO() {
		glGenVertexArrays(1, &ID);
	}
	~VAO() {
		Delete();
	}

	void LinkAttrib(VBO<T>& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	void Bind() {
		glBindVertexArray(ID);
	}

	void Unbind() {
		glBindVertexArray(0);
	}

	void Delete() {
		glDeleteVertexArrays(1, &ID);
	}
};

#endif;

