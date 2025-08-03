#ifndef WORLD_H
#define WORLD_H


#include <thread>
#include <mutex>


#include "ShaderManager.h"
#include "MyChunk.h"
#include "VoxelHandler.h"
#include <unordered_set>
#include <queue>


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
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//The red block in the face is annoying
	}

};



class World {
public:


	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>> chunks = {};
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>> unsafeChunks = {};

	Camera* camera;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraOrientation;

	FastNoiseLite noise;

	VoxelHandler voxelHandler;
	VoxelMarker voxelMarker = VoxelMarker(&voxelHandler);


	std::deque<ChunkCoord> ChunkLoadList;
	std::deque<ChunkCoord> ChunkRenderList;
	std::vector<ChunkCoord> ChunkUnloadList;

	World();
	~World();
	void setCamera(Camera* camera);

	void buildChunks();

	void update();

	void updateChunks();
	void deleteChunks();
	void ThreadInsert();

	void loadChunks();

	void render();

	void UpdateChunkThread();

private:
	std::thread chunkThread;
	std::mutex chunkMutex;
	bool isThreadRunning = false;

	bool ChunkExists(const ChunkCoord chunkCoord);
};



#endif;
