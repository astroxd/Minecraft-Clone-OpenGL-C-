#include "VoxelHandler.h"

#include <iostream>

#include <cmath>

void VoxelHandler::init(Camera* camera) {
	VoxelHandler::camera = camera;
}

void VoxelHandler::rayCasting() {
	float maxX, maxY, maxZ, deltaX, deltaY, deltaZ;

	//WORKS WITH NEGATIVES TOO
	//float x1 = abs(camera->Position.x);
	//float y1 = abs(camera->Position.y);
	//float z1 = abs(camera->Position.z);

	float x1 = camera->Position.x;
	float y1 = camera->Position.y;
	float z1 = camera->Position.z;
	
	auto normOri = glm::normalize(camera->Orientation);
	
	float x2 = x1 + camera->Orientation.x * MAX_RAY_DIST;
	float y2 = y1 + camera->Orientation.y * MAX_RAY_DIST;
	float z2 = z1 + camera->Orientation.z * MAX_RAY_DIST;

	glm::vec3 currentVoxelPos = glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z);
	voxelId = 0;
	voxelNormal = glm::vec3(0);
	int stepDir = -1;

	int dx = glm::sign(x2 - x1);
	if (dx != 0) deltaX = std::min(dx / (x2 - x1), 10000000.0f); else deltaX = 10000000.0f;
	if (dx > 0) maxX = deltaX * (1.0f - glm::fract(x1)); else maxX = deltaX * glm::fract(x1);

	int dy = glm::sign(y2 - y1);
	if (dy != 0) deltaY = std::min(dy / (y2 - y1), 10000000.0f); else deltaY = 10000000.0f;
	if (dy > 0) maxY = deltaY * (1.0f - glm::fract(y1)); else maxY = deltaY * glm::fract(y1);

	int dz = glm::sign(z2 - z1);
	if (dz != 0) deltaZ = std::min(dz / (z2 - z1), 10000000.0f); else deltaZ = 10000000.0f;
	if (dz > 0) maxZ = deltaZ * (1.0f - glm::fract(z1)); else maxZ = deltaZ * glm::fract(z1);

	//std::cout << "X: " << maxX << " Y: " << maxY << " Z: " << maxZ << std::endl;
	while (not(maxX > 1.0f && maxY > 1.0f && maxZ > 1.0f)) {
		std::vector<int> values = getVoxelId(currentVoxelPos);
		
		//voxelId = values[0];
		if (values[0] > 0) {
			voxelId = values[0];
			std::cout << "VOXELID: " << voxelId << std::endl;
			voxelLocalPosition = glm::ivec3(values[1], values[2], values[3]);
			chunkCoord = { values[4], values[5] };
			voxelWorldPos = currentVoxelPos;

			if (stepDir == 0) {
				voxelNormal.x = -dx;
			}
			else if (stepDir == 1) {
				voxelNormal.y = -dy;
			}
			else {
				voxelNormal.z = -dz;
			}
			return;
		}

		if (maxX < maxY) {
			if (maxX < maxZ) {
				currentVoxelPos.x += dx;
				maxX += deltaX;
				stepDir = 0;
			}
			else {
				currentVoxelPos.z += dz;
				maxZ += deltaZ;
				stepDir = 2;
			}
		}
		else {
			if (maxY < maxZ) {
				currentVoxelPos.y += dy;
				maxY += deltaY;
				stepDir = 1;
			}
			else {
				currentVoxelPos.z += dz;
				maxZ += deltaZ;
				stepDir = 2;
			}
		}

	}

	return;
}

std::vector<int> VoxelHandler::getVoxelId(glm::vec3 voxelWorldPos) {

	int cx = floor(voxelWorldPos.x / CHUNK_W);
	int cy = floor(voxelWorldPos.y / CHUNK_H);
	int cz = floor(voxelWorldPos.z / CHUNK_D);

	//if (cx == -0) cx == -1;
	//if (cz == -0) cz == -1;
	ChunkCoord coord = { cx, cz };

	auto chunk = chunks->find(coord);
	if (chunk != chunks->end()) {

		int lx = abs(static_cast<int>(voxelWorldPos.x) % CHUNK_W);
		int ly = abs(static_cast<int>(voxelWorldPos.y) % CHUNK_H);
		int lz = abs(static_cast<int>(voxelWorldPos.z) % CHUNK_D);

		 
		if (coord.first < 0) lx = (CHUNK_W - 1) - lx;
		if (coord.second < 0) lz = (CHUNK_D - 1) - lz;

		int voxelId = chunk->second->blocks[lx][lz][ly];
		//std::cout << "WORLD POS X: " << voxelWorldPos.x << " WORLD POS Z: " << voxelWorldPos.z << std::endl;
		//std::cout << "LOCAL X: " << lx << " LOCAL Z: " << lz << std::endl;;
		

		int values[] = { voxelId, lx, ly, lz, coord.first, coord.second };
		std::vector<int> vec(values, values + sizeof(values) / sizeof(int));
		return vec;
	}
	
	int values[6] = { 0,0,0,0,0,0 };
	std::vector<int> vec(values, values + sizeof(values) / sizeof(int));
	return vec;



}

void VoxelHandler::destroyVoxel() {
	std::chrono::milliseconds time = getMs();
	bool canChangeBlock = false;

	if ((time - lastDestroyed).count() > DELAY_DESTROY_BLOCK) canChangeBlock = true;

	if (!canChangeBlock) return;

	if (voxelId > 0) {
		(*chunks)[chunkCoord]->blocks[voxelLocalPosition.x][voxelLocalPosition.z][voxelLocalPosition.y] = 0;
		(*chunks)[chunkCoord]->generateChunk();
		(*chunks)[chunkCoord]->setVAO();

		if (voxelLocalPosition.x == 0) {
			(*chunks)[{chunkCoord.first - 1, chunkCoord.second}]->generateChunk();
			(*chunks)[{chunkCoord.first - 1, chunkCoord.second}]->setVAO();
		}
		else if (voxelLocalPosition.x == CHUNK_W - 1) {
			(*chunks)[{chunkCoord.first + 1, chunkCoord.second}]->generateChunk();
			(*chunks)[{chunkCoord.first + 1, chunkCoord.second}]->setVAO();
		}

		if (voxelLocalPosition.z == 0) {
			(*chunks)[{chunkCoord.first, chunkCoord.second - 1}]->generateChunk();
			(*chunks)[{chunkCoord.first, chunkCoord.second - 1}]->setVAO();
		}
		else if (voxelLocalPosition.z == CHUNK_D - 1) {
			(*chunks)[{chunkCoord.first, chunkCoord.second + 1}]->generateChunk();
			(*chunks)[{chunkCoord.first, chunkCoord.second + 1}]->setVAO();
		}
		lastDestroyed = time;
	}
}

void VoxelHandler::placeVoxel() {
	std::chrono::milliseconds time = getMs();
	bool canChangeBlock = false;

	if ((time - lastPlaced).count() > DELAY_PLACE_BLOCK) canChangeBlock = true;

	if (!canChangeBlock) return;

	if (voxelId > 0) {
		std::vector<int> values = getVoxelId(voxelWorldPos + voxelNormal);
		if (values[0] == 0) {
			//std::cout << "ENTRA " << std::endl;
			//std::cout << " LOCAL X: " << values[1] << " LOCAL Y: " << values[2] << " LOCAL Z: " << values[3] << std::endl;
			(*chunks)[{values[4], values[5]}]->blocks[values[1]][values[3]][values[2]] = 1;
			(*chunks)[{values[4], values[5]}]->generateChunk();
			(*chunks)[{values[4], values[5]}]->setVAO();
		}
	}
	lastPlaced = time;
}

void VoxelHandler::input(GLFWwindow* window) {
	if (window == NULL) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//placeVoxel();
		destroyVoxel();
	}
}

void VoxelHandler::update(std::map<ChunkCoord, std::unique_ptr<MyChunk>>* chunks) {
	VoxelHandler::chunks = chunks;
	rayCasting();
}



std::chrono::milliseconds VoxelHandler::getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}

