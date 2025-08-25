#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Texture.h"


template <typename VertexType>
class Mesh {

public:
	std::vector <VertexType> m_Vertices;
	std::vector <GLuint> m_Indices;
	std::vector <Texture> texture;

	VAO<VertexType> VAO;
	VBO<VertexType> VBO;
	EBO EBO;

	Mesh() = default;
	virtual ~Mesh()
	{
		VAO.Delete();
		VBO.Delete();
		EBO.Delete();
	}

	// Sets vertices and indices
	virtual void SetVAO() = 0;

	// Draws the mesh
	virtual void Draw() = 0;
};
#endif;
