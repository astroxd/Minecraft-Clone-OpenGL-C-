#include "Chunk.h"
#include "Log.h"
#include "ShaderManager.h"



Chunk::Chunk() {}

Chunk::Chunk(ChunkCoord coord, glm::vec3 position, FastNoiseLite* noise)
	: m_Coord(coord), m_Position(glm::vec3(position.x* CHUNK_W, position.y* CHUNK_H, position.z* CHUNK_D)), m_Noise(noise)
{
	m_Blocks = new unsigned int[CHUNK_W * CHUNK_D * CHUNK_H];
	GenerateBlocks();

}

void Chunk::GenerateBlocks() {

	int random = rand() % 100 + 1;

	for (size_t x = 0; x < CHUNK_W; x++)
	{
		int wx = x + m_Position.x;

		for (size_t z = 0; z < CHUNK_D; z++)
		{
			int wz = z + m_Position.z;
			int height = ((1 + (m_Noise->GetNoise(float(wx), float(wz)))) / 2) * CHUNK_H;
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
					//blocks[x][z][y] = x + z + y + 1;
					//blocks[x][z][y] = abs(coord.first + coord.second) + 1;
					SetBlock(x, z, y, random);
				}
				else {
					SetBlock(x, z, y, 0);
				}

			}
		}
	}
}


void Chunk::SetWorldChunks(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* worldChunks)
{
	this->m_WorldChunks = worldChunks;

}


bool Chunk::GetNeighbourChunkIndex(AdjacentChunkPos pos, int neighbourBlockX, int neighbourBlockZ, int y) {
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>::iterator neighbourChunk;

	neighbourChunk = m_WorldChunks->find(m_Coord + s_AdjacentChunks[pos]);
	if (neighbourChunk == m_WorldChunks->end()) {
		return false;
	}

	unsigned int neighbourBlock = neighbourChunk->second->GetBlock(neighbourBlockX, neighbourBlockZ, y);
	if (neighbourBlock > 0) return false;

	return true;

}

bool Chunk::CheckIfVoid(int x, int z, int y) {
	if (y < 0) return true;
	//LEFT FACE
	if (x == -1) {
		if (z == -1) {
			return GetNeighbourChunkIndex(TOPRIGHT, CHUNK_W - 1, CHUNK_D - 1, y);
		}
		else if (z == CHUNK_D) {
			return GetNeighbourChunkIndex(TOPLEFT, CHUNK_W - 1, 0, y);
		}
		else {
			return GetNeighbourChunkIndex(XNEG, CHUNK_W - 1, z, y);
		}
	}

	//RIGHT FACE
	else if (x == CHUNK_W) {
		if (z == -1) {
			return GetNeighbourChunkIndex(BOTTOMRIGHT, 0, CHUNK_D - 1, y);
		}
		else if (z == CHUNK_D) {
			return GetNeighbourChunkIndex(BOTTOMLEFT, 0, 0, y);
		}
		else {
			return GetNeighbourChunkIndex(XPOS, 0, z, y);
		}
	}

	//FRONT FACE
	else if (z == -1) {
		return GetNeighbourChunkIndex(ZNEG, x, CHUNK_D - 1, y);
	}

	//BACK FACE
	else if (z == CHUNK_D) {
		return GetNeighbourChunkIndex(ZPOS, x, 0, y);
	}

	//INSIDE
	else if (x >= 0 && x < CHUNK_W && y >= 0 && y < CHUNK_H && z >= 0 && z < CHUNK_D) {
		unsigned int block = GetBlock(x, z, y);
		if (block > 0) return false;
	}

	return true;
}


std::vector<int> Chunk::GetAo(int x, int z, int y, char plane) {
	bool a, b, c, d, e, f, g, h;

	if (plane == 'Y') {
		a = CheckIfVoid(x, z - 1, y);
		b = CheckIfVoid(x - 1, z - 1, y);
		c = CheckIfVoid(x - 1, z, y);
		d = CheckIfVoid(x - 1, z + 1, y);
		e = CheckIfVoid(x, z + 1, y);
		f = CheckIfVoid(x + 1, z + 1, y);
		g = CheckIfVoid(x + 1, z, y);
		h = CheckIfVoid(x + 1, z - 1, y);
	}
	else if (plane == 'X') {
		a = CheckIfVoid(x, z - 1, y);
		b = CheckIfVoid(x, z - 1, y - 1);
		c = CheckIfVoid(x, z, y - 1);
		d = CheckIfVoid(x, z + 1, y - 1);
		e = CheckIfVoid(x, z + 1, y);
		f = CheckIfVoid(x, z + 1, y + 1);
		g = CheckIfVoid(x, z, y + 1);
		h = CheckIfVoid(x, z - 1, y + 1);
	}
	else {
		a = CheckIfVoid(x - 1, z, y);
		b = CheckIfVoid(x - 1, z, y - 1);
		c = CheckIfVoid(x, z, y - 1);
		d = CheckIfVoid(x + 1, z, y - 1);
		e = CheckIfVoid(x + 1, z, y);
		f = CheckIfVoid(x + 1, z, y + 1);
		g = CheckIfVoid(x, z, y + 1);
		h = CheckIfVoid(x - 1, z, y + 1);
	}

	std::vector<int> ao = {
		c + d + e, // BOTTOM LEFT 
		e + f + g, // BOTTOM RIGHT
		g + h + a, // TOP RIGHT
		a + b + c  // TOP LEFT
	};

	return ao;
}


void Chunk::GenerateChunk() {
	//LOG_TRACE("GENERATING CHUNK");

	vertices.clear();
	indices.clear();
	m_CountIndices = 0;
	std::vector<int> ao = { 1, 1, 1, 1 };
	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int z = 0; z < CHUNK_D; z++)
		{
			for (int y = 0; y < CHUNK_H; y++)
			{
				unsigned int voxelId = GetBlock(x, z, y);
				if (voxelId <= 0) continue;


				//TOP FACE
				if (CheckIfVoid(x, z, y + 1)) {
					auto ao = GetAo(x, z, y + 1, 'Y');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::TOP_FACE, ao);
				}

				//BOTTOM FACE
				if (CheckIfVoid(x, z, y - 1)) {
					auto ao = GetAo(x, z, y - 1, 'Y');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::BOTTOM_FACE, ao);
				}

				//RIGHT FACE
				if (CheckIfVoid(x + 1, z, y)) {
					auto ao = GetAo(x + 1, z, y, 'X');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::RIGHT_FACE, ao);
				}

				//LEFT FACE
				if (CheckIfVoid(x - 1, z, y)) {
					auto ao = GetAo(x - 1, z, y, 'X');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::LEFT_FACE, ao);
				}

				//FRONT FACE
				if (CheckIfVoid(x, z + 1, y)) {
					auto ao = GetAo(x, z + 1, y, 'Z');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::FRONT_FACE, ao);
				}
				//BACK FACE
				if (CheckIfVoid(x, z - 1, y)) {
					auto ao = GetAo(x, z - 1, y, 'Z');
					GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::BACK_FACE, ao);
				}
			}
		}
	}
	//setVAO();
}

void Chunk::GenerateFace(glm::vec3 position, unsigned int voxelId, BlockFace face, std::vector<int> ao) {
	std::vector<glm::vec3> rawVertices = rawVertexData.at(face);
	for (int i = 0; i < rawVertices.size(); i++)
	{
		vertices.push_back(Vertex{ rawVertices[i] + position, voxelId, (unsigned int)face,  glm::vec2(0,0) , ao[i] });
	}

	//TODO check this after texture implementation
	bool flipId = ao[2] + ao[0] > ao[3] + ao[1];
	if (flipId) {
		indices.push_back(m_CountIndices);
		indices.push_back(m_CountIndices + 1);
		indices.push_back(m_CountIndices + 3);

		indices.push_back(m_CountIndices + 1);
		indices.push_back(m_CountIndices + 2);
		indices.push_back(m_CountIndices + 3);
	}
	else {

		indices.push_back(m_CountIndices);
		indices.push_back(m_CountIndices + 1);
		indices.push_back(m_CountIndices + 2);

		indices.push_back(m_CountIndices);
		indices.push_back(m_CountIndices + 2);
		indices.push_back(m_CountIndices + 3);
	}
	m_CountIndices += 4;
}

unsigned int Chunk::GetBlock(int x, int z, int y) const {
	return m_Blocks[GetBlockIndex(x, z, y)];
}
void Chunk::SetBlock(int x, int z, int y, unsigned int blockId) {
	if (y > CHUNK_H || y < 0) return; //TEMPORARY FIX
	m_Blocks[GetBlockIndex(x, z, y)] = blockId;
}

void Chunk::SetBlock(glm::ivec3 blockPos, unsigned int blockId) {
	if (blockPos.y > CHUNK_H || blockPos.y < 0) return; //TEMPORARY FIX
	m_Blocks[GetBlockIndex(blockPos)] = blockId;
}




void Chunk::Reset() {
	isMeshing = false;
	isMeshed = false;
	isLoaded = false;
}



void Chunk::Render(Camera* camera) {
	//if (!isBuilt) return;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_Position);

	Shader shader = ShaderManager::GetShader("ShaderProgram");

	shader.Activate();
	shader.SetMat4("model", model);
	//texture[0].texUnit(shader, "tex0", 0);
	//texture[0].Bind();
	Draw(shader);
}
