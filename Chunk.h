#ifndef CHUNKK_H
#define CHUNKK_H

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


enum AdjacentChunkPos {
	XPOS,
	XNEG,
	ZPOS,
	ZNEG
};

class Chunk : public Mesh {
public:

	bool isBuilt = false; //IL CHUNK PUò ESSERE RENDERIZZATO
	bool forceRebuid = false;
	bool isMeshed = false; //POSSO CHIAMARE IL VAO PERCHé HO GENERATO TUTTE LE FACCE
	bool isLoaded = false; //SIGNIFICA CHE è GIà STATO NELLA CHUNKLOADLIST
	bool isMeshing = false; //è GIà STATO CHIAMATO generateChunks MA STA ANCORA PROCESSANDO



	Chunk();
	explicit Chunk(ChunkCoord coord, glm::vec3 position, FastNoiseLite* noise);

	~Chunk() {
		LOG_INFO("DELETING CHUNK");
		delete[] m_Blocks;
	};

	void GenerateBlocks();

	void SetWorldChunks(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* worldChunks);
	void GenerateChunk();

	const inline ChunkCoord& GetChunkCoord() { return m_Coord; }
	unsigned int GetBlock(int x, int z, int y) const;
	void SetBlock(int x, int z, int y, unsigned int blockId);

	void Reset();
	void Render(Camera* camera);

	static inline ChunkCoord GetChunkCoordFromWorldCoord(int worldX, int worldZ) {
		return std::make_pair(worldX, worldZ);
	}

	static inline unsigned int GetBlockIndex(int x, int z, int y) {
		return x + CHUNK_W * (y + CHUNK_H * z);
	}

private:
	unsigned int* m_Blocks;
	glm::vec3 m_Position;
	ChunkCoord m_Coord;
	FastNoiseLite* m_Noise;

	ChunkCoord m_AdjacentChunks[4] = {
		std::make_pair(m_Coord.first + 1, m_Coord.second),
		std::make_pair(m_Coord.first - 1, m_Coord.second),
		std::make_pair(m_Coord.first, m_Coord.second + 1),
		std::make_pair(m_Coord.first, m_Coord.second - 1)
	};

	int m_CountIndices = 0;
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* m_WorldChunks;

private:
	bool CheckIfVoid(int x, int z, int y);
	bool GetNeighbourChunkIndex(AdjacentChunkPos pos, int neighbourBlockX, int neighbourBlockZ, int y);
	std::vector<int> GetAo(int x, int z, int y, char plane);

	void GenerateFace(glm::vec3 position, unsigned int voxelId, BlockFace faceId, std::vector<int> ao);

};


#endif