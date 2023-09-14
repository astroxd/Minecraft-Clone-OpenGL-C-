#ifndef MY_CHUNK_H
#define MY_CHUNK_H

#include "Mesh.h"
#include "FastNoiseLite.h"
#include "Camera.h"

#include <map>

const int CHUNK_W = 16;
const int CHUNK_D = 16;
const int CHUNK_H = 50;

typedef std::pair<int, int> ChunkCoord;


class MyChunk : public Mesh {
public:
	//std::vector<unsigned int> blocks;

	unsigned int blocks[CHUNK_W][CHUNK_D][CHUNK_H] = {};

	ChunkCoord coord;

	bool isVisible = false;
	bool isBuilt = false;

	glm::vec3 position;

	std::map<ChunkCoord, std::unique_ptr<MyChunk>>* worldChunks;

	FastNoiseLite* noise;

	MyChunk();
	explicit MyChunk(ChunkCoord coord, glm::vec3 position, Shader& shader, FastNoiseLite* noise);

	~MyChunk() {
		
	};

	void setWorldChunks(std::map<ChunkCoord, std::unique_ptr<MyChunk>>* worldChunks);

	void generateBlocks();
	void generateChunk();
	bool checkIfVoid(int x, int z, int y);
	bool getNeighbourChunkIndex(ChunkCoord neighbourChunkCoord, int neighbourBlockX, int neighbourBlockZ, int y);
	void render(Shader& shader, Camera* camera);
};


#endif