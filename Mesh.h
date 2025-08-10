#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Texture.h"


class Mesh {

public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> texture;

	// Store VAO in public so it can be used in the Draw function
	VAO VAO;
	VBO VBO;
	EBO EBO;
	// Initializes the mesh
	Mesh();
	Mesh(std::vector <Vertex>& vertices);
	virtual ~Mesh()
	{
		VAO.Delete();
		VBO.Delete();
		EBO.Delete();
	}
	void setData(std::vector <Vertex>& vertices);
	virtual void setVAO();
	//Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& texture);

	//Mesh(std::vector <Vertex>& vertices, std::vector <Texture>& texture);

	//void setData(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& texture);

	// Draws the mesh
	void Draw(Shader& shader);
};
#endif;
