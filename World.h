#ifndef WORLD_H
#define WORLD_H

#include "MyChunk.h"
#include "VoxelHandler.h"

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
	
	const int MAX_RAY_DIST = 6;
	bool rayCasting();
	int getVoxelId(glm::vec3 voxelWorldPos);

};



#endif;
