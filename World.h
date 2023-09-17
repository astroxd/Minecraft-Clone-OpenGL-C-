#ifndef WORLD_H
#define WORLD_H

#include "MyChunk.h"
#include "VoxelHandler.h"


class VoxelMarker : public Mesh {
public:

	VoxelHandler* handler;

	VoxelMarker(VoxelHandler* handler) {
		VoxelMarker::handler = handler;
		generateVertices();
		setVAO();
	};


	void setVAO() override {
		VAO.Bind();
		VBO.setVertices(vertices);

		// Links VBO attributes such as coordinates and colors to VAO
		VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		// Unbind all to prevent accidentally modifying them
		VAO.Unbind();
		VBO.Unbind();
	};

	void generateVertices() {
		vertices.clear();
		vertices = {
			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },

			Vertex{ glm::vec3(0.0f, 0.0f,  1.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f,  1.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 1.0f) },

			Vertex{ glm::vec3(0.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 1.0f) },

			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },

			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(0.5f, 0.0f, 0.0f) },
			Vertex{ glm::vec3(0.5f, 0.0f, 1.0f) },
			Vertex{ glm::vec3(0.5f, 0.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 0.0f, 0.0f) },

			Vertex{ glm::vec3(0.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 0.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 1.0f) },
			Vertex{ glm::vec3(0.0f, 1.0f, 0.0f) } 
		};
	};
	
	void render(Shader& shader, glm::vec3 position) {
		if (handler->voxelId == 0) return;

		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::translate(model, floor(handler->voxelWorldPos));

		shader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Draw(shader);
	}

};



class World {
public:

	//std::vector<MyChunk> chunks;
	//string = xz
	std::map<ChunkCoord, std::unique_ptr<MyChunk>> chunks = {};
	
	//std::vector<std::vector<unsigned int>> voxels;

	Shader shader;

	Camera* camera;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraOrientation;

	FastNoiseLite noise;
	GLFWwindow* window;

	VoxelHandler voxelHandler;
	VoxelMarker voxelMarker = VoxelMarker(&voxelHandler);

	World() {
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		noise.SetSeed(122422);
	};

	void setShader(Shader& shader);
	void setCamera(Camera* camera, GLFWwindow* window);

	void buildChunks();

	void update();

	void updateChunks();
	void deleteChunks();

	void render(Shader& shader);

};



#endif;
