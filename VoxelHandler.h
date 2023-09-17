#ifndef VOXEL_HANDLER_H
#define VOXEL_HANDLER_H

#include "MyChunk.h"
#include <chrono>

class VoxelHandler {
public:

	const int MAX_RAY_DIST = 12;
	const int DELAY_DESTROY_BLOCK = 100;
	const int DELAY_PLACE_BLOCK = 100;
	Camera* camera;
	std::map<ChunkCoord, std::unique_ptr<MyChunk>>* chunks;

	int voxelId = 0;
	std::chrono::milliseconds lastDestroyed;
	std::chrono::milliseconds lastPlaced;

	ChunkCoord chunkCoord;
	glm::ivec3 voxelLocalPosition;
	glm::vec3 voxelNormal;
	glm::vec3 voxelWorldPos;

	VoxelHandler() {};
	void init(Camera* camera);

	void rayCasting();
	std::vector<int> getVoxelId(glm::vec3 voxelWorldPos);

	void destroyVoxel();
	void placeVoxel();


	void input(GLFWwindow* window);
	void update(std::map<ChunkCoord, std::unique_ptr<MyChunk>>* chunks);
	

private:
	std::chrono::milliseconds getMs();
};

#endif;
