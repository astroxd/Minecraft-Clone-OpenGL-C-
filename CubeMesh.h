/*#ifndef CUBEMESH_H
#define CUBEMESH_H


#include "Mesh.h"
#include "Shader.h"




class CubeMesh : public Mesh{

	public:

	enum Blocks {
		Grass,
		Cobblestone
	};

	std::vector<Vertex> vertices {
		Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(0.0f, 0.0f)},	// Front Bottom Left	0
		Vertex{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(0.0f, 1.0f)},	// Front Bottom Right	1
		Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(0.0f, 1.0f)},	// Front Top Left		2
		Vertex{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(1.0f, 1.0f)},	// Front Top Right		3

		Vertex{glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(0.0f, 0.0f)}, // Back Bottom Left		4
		Vertex{glm::vec3(0.5f, -0.5f,  -0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(1.0f, 0.0f)}, // Back Bottom Right		5
		Vertex{glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(0.0f, 1.0f)}, // Back Top Left			6
		Vertex{glm::vec3(0.5f,  0.5f,  -0.5f), glm::vec3(0.8f,  0.3f, 0.02f), glm::vec2(1.0f, 1.0f)}, // Back Top Right		7
	};

	std::vector<GLuint> indices {
		//Bottom
		0, 4, 5,
		5, 1, 0,

		//Front
		0, 1, 3,
		3, 2, 0,

		//Left
		4, 0, 2,
		2, 6, 4,


		//Right
		1, 5, 7,
		7, 3, 1,

		//Back
		5, 4, 6,
		6, 7, 5,

		//Top
		2, 3, 7,
		7, 6, 2,
	};
	std::vector<Texture> textures;

	CubeMesh(Blocks block);
};


#endif // !CUBEMESH_H
*/