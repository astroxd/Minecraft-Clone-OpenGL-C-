#ifndef MY_CHUNK_H
#define MY_CHUNK_H

#include <iostream>
#include <unordered_map>

#include "Mesh.h"
#include "FastNoiseLite.h"
#include "Camera.h"
#include "Block.h"
#include "Log.h"

const int CHUNK_W = 16;
const int CHUNK_D = 16;
const int CHUNK_H = 16;

typedef std::pair<int, int> ChunkCoord;

// A hash function used to hash a pair of any kind
struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		// Hash the first element
		size_t hash1 = std::hash<T1>{}(p.first);
		// Hash the second element
		size_t hash2 = std::hash<T2>{}(p.second);
		// Combine the two hash values
		return hash1
			^ (hash2 + 0x9e3779b9 + (hash1 << 6)
				+ (hash1 >> 2));
	}
};

template<typename Key, typename Value>
using ChunkUnorderedMap = std::unordered_map<Key, Value, hash_pair>;


class MyChunk : public Mesh {
public:

	//TODO HEAP allocate this
	unsigned int blocks[CHUNK_W][CHUNK_D][CHUNK_H] = {};

	ChunkCoord coord;
	glm::vec3 position;

	bool isVisible = false;
	bool isBuilt = false; //IL CHUNK PUò ESSERE RENDERIZZATO
	bool forceRebuid = false;
	bool isMeshed = false; //POSSO CHIAMARE IL VAO PERCHé HO GENERATO TUTTE LE FACCE
	bool isLoaded = false; //SIGNIFICA CHE è GIà STATO NELLA CHUNKLOADLIST
	bool isMeshing = false; //è GIà STATO CHIAMATO generateChunks MA STA ANCORA PROCESSANDO
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* worldChunks;
	FastNoiseLite* noise;

	MyChunk();
	explicit MyChunk(ChunkCoord coord, glm::vec3 position, FastNoiseLite* noise);

	~MyChunk() {
		LOG_INFO("DELETING CHUNK");
	};

	void setWorldChunks(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* worldChunks);

	void generateBlocks();
	void generateChunk();
	bool checkIfVoid(int x, int z, int y);
	bool getNeighbourChunkIndex(ChunkCoord neighbourChunkCoord, int neighbourBlockX, int neighbourBlockZ, int y);
	void render(Camera* camera);

	std::vector<int> getAo(int x, int z, int y, char plane);

	static inline ChunkCoord getChunkCoordFromWorldCoord(int worldX, int worldZ) {
		return std::make_pair(worldX, worldZ);
	}

	int countIndices = 0;
	void generateFace(glm::vec3 position, unsigned int voxelId, BlockFace faceId, std::vector<int> ao);


};


#endif