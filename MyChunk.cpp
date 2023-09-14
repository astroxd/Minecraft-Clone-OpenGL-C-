#include "MyChunk.h"

//#include <glm/ext.hpp>

#include <iostream>

MyChunk::MyChunk() {
	//std::cout << "Chunk created" << std::endl;

	//generateBlocks();
}

MyChunk::MyChunk(ChunkCoord coord, glm::vec3 position, Shader& shader, FastNoiseLite* noise) {
	MyChunk::coord = coord;

	MyChunk::position = glm::vec3(position.x * CHUNK_W, position.y * CHUNK_H, position.z * CHUNK_D);
	
	MyChunk::noise = noise;
	
	generateBlocks();

}


void MyChunk::setWorldChunks(std::map<ChunkCoord, std::unique_ptr<MyChunk>>* worldChunks) {
	MyChunk::worldChunks = worldChunks;

	generateChunk();

}

void MyChunk::generateBlocks() {
	//texture.push_back(Texture("dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));

	for (size_t x = 0; x < CHUNK_W; x++)
	{
		int wx = x + position.x;

		for (size_t z = 0; z < CHUNK_D; z++)
		{
			int wz = z + position.z;
			int height = 20 + (noise->GetNoise(float(wx), float(wz)))  * 20;
			//std::cout << "NOISE: " << height << std::endl;
			//TODO add frustum culling
			for (size_t y = 0; y < CHUNK_H; y++)
			{
				/*if (y > height) {
					blocks[x][z][y] = 0;
				}
				else {

				blocks[x][z][y] = x + z + y + 1;
				}*/
				
				if (y <= height) {
					blocks[x][z][y] = x + z + y + 1;
					//blocks[x][z][y] = abs(coord.first + coord.second) + 1;
				}
				else {
					blocks[x][z][y] = 0;
				}
				
			}
		}
	}
	
	//generateChunk();
}

bool MyChunk::getNeighbourChunkIndex(ChunkCoord neighbourChunkCoord, int neighbourBlockX, int neighbourBlockZ, int y) {
	std::map<ChunkCoord, std::unique_ptr<MyChunk>>::iterator neighbourChunk;

	neighbourChunk = (*worldChunks).find(neighbourChunkCoord);
	if (neighbourChunk == (*worldChunks).end()) {
		return true;
	}

	int neighbourBlock = neighbourChunk->second->blocks[neighbourBlockX][neighbourBlockZ][y];
	if (neighbourBlock > 0) return false;

}



bool MyChunk::checkIfVoid(int x, int z, int y) {
	
	//LEFT FACE
	if (x == -1) {
		return getNeighbourChunkIndex({ coord.first - 1, coord.second}, CHUNK_W - 1, z, y);
	}

	//RIGHT FACE
	else if (x == CHUNK_W) {
		return getNeighbourChunkIndex({ coord.first + 1, coord.second }, 0, z, y);
	}

	//FRONT FACE
	else if (z == -1) {
		return getNeighbourChunkIndex({ coord.first, coord.second - 1 }, x, CHUNK_D - 1, y);
	}

	//BACK FACE
	else if (z == CHUNK_D) {
		return getNeighbourChunkIndex({ coord.first, coord.second + 1 }, x, 0, y);
	}
	
	//INSIDE
	else if (x >= 0 && x < CHUNK_W && y >= 0 && y < CHUNK_H && z >= 0 && z < CHUNK_D) {
		int block = blocks[x][z][y];
		if (block > 0) return false;
	}

	return true;
}

void MyChunk::generateChunk() {
	std::cout << "GENERATING CHUNK" << std::endl;
	//if (!isVisible) return;
	vertices.clear();
	//std::cout << "SIZE: " << worldChunks->size() << std::endl;
	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int z = 0; z < CHUNK_D; z++)
		{
			for (int y = 0; y < CHUNK_H; y++)
			{
				unsigned int voxelId = blocks[x][z][y];
				if (voxelId <= 0) continue;

					//LEFT FACE
					if (checkIfVoid(x - 1, z, y)) {
						vertices.push_back(Vertex{ glm::vec3(x, y    , z), voxelId, 3, glm::vec2(0,0)});	//v0
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z + 1), voxelId, 3, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z), voxelId, 3, glm::vec2(1,0) });	//v1

						vertices.push_back(Vertex{ glm::vec3(x, y    , z), voxelId, 3, glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x, y    , z + 1), voxelId, 3, glm::vec2(1,1) });	//v3
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z + 1), voxelId, 3, glm::vec2(0,1) });	//v2
					}

					//RIGHT FACE
					if (checkIfVoid(x + 1, z, y)) {
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 2,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 2, glm::vec2(1,0) });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 2, glm::vec2(0,1) });	//v2

						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 2,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 2, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 2, glm::vec2(1,1) });	//v3
					
					}

					//TOP FACE
					if (checkIfVoid(x, z, y + 1)) {
						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z), voxelId, 0,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z + 1), voxelId, 0, glm::vec2(1,1) });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z + 1), voxelId, 0, glm::vec2(0,1) });	//v2

						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z), voxelId, 0,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z + 1), voxelId, 0, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z), voxelId, 0, glm::vec2(1,0) });

					}

					//BOTTOM FACE
					if (checkIfVoid(x, z, y - 1)) {
						vertices.push_back(Vertex{ glm::vec3(x    , y, z), voxelId, 1,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z + 1), voxelId, 1, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x    , y, z + 1), voxelId, 1, glm::vec2(1,1) });	//v3

						vertices.push_back(Vertex{ glm::vec3(x    , y, z), voxelId, 1,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z), voxelId, 1, glm::vec2(1,0) });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z + 1), voxelId, 1, glm::vec2(0,1) });	//v2

					}

					//FRONT FACE
					if (checkIfVoid(x, z + 1, y)) {
						vertices.push_back(Vertex{ glm::vec3(x    , y    , z + 1), voxelId, 5,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 5, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z + 1), voxelId, 5, glm::vec2(1,0) });	//v1

						vertices.push_back(Vertex{ glm::vec3(x    , y    , z + 1), voxelId, 5,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 5,glm::vec2(1,1) });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 5, glm::vec2(0,1) });	//v2

					}

					//BACK FACE
					if (checkIfVoid(x, z - 1, y)) {
						vertices.push_back(Vertex{ glm::vec3(x    , y    , z), voxelId, 4,  glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z), voxelId, 4, glm::vec2(1,0) });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 4, glm::vec2(0,1) });	//v2

						vertices.push_back(Vertex{ glm::vec3(x    , y    , z), voxelId, 4, glm::vec2(0,0) });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 4, glm::vec2(0,1) });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 4, glm::vec2(1,1) });	//v3

					}

					
			
			}
		}
	}

	//setVAO();
}

void MyChunk::render(Shader& shader, Camera* camera){
	//if (!camera->isOnFrustum(position)) return;
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	

	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//texture[0].texUnit(shader, "tex0", 0);
	//texture[0].Bind();
	Draw(shader);
}

