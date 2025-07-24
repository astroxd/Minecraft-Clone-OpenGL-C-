#ifndef WORLD_H
#define WORLD_H

#include "ShaderManager.h"
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

	void render(glm::vec3 position) {
		if (handler->voxelId == 0) return;
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, floor(handler->voxelWorldPos));

		Shader shader = ShaderManager::GetShader("VoxelMarkerProgram");
		shader.Activate();
		shader.SetMat4("model", model);
		VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

};



class World {
public:


	ChunkUnorderedMap<ChunkCoord, std::unique_ptr<MyChunk>> chunks = {};

	Camera* camera;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraOrientation;

	FastNoiseLite noise;

	VoxelHandler voxelHandler;
	VoxelMarker voxelMarker = VoxelMarker(&voxelHandler);

	World() {
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		noise.SetSeed(122422);
		chunks.reserve(13 * 13);
	};

	void setCamera(Camera* camera);

	void buildChunks();

	void update();

	void updateChunks();
	void deleteChunks();

	void render();

};



#endif;
