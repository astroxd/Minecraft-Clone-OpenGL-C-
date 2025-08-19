#include "Chunk.h"
#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"


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
			height = height - 2 > 0 ? height : height + 2;
			for (size_t y = 0; y < CHUNK_H; y++)
			{
				if (y == 0) {
					SetBlock(x, z, y, BEDROCK);
				}
				else if (y > 0 && y < height - 2) {
					SetBlock(x, z, y, COBBLESTONE);
				}
				else if (y < height) {
					SetBlock(x, z, y, DIRT);
				}
				else if (y == height) {
					SetBlock(x, z, y, GRASS);
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
	if (y < 0 || y >= CHUNK_H) return true;
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
		int block = GetBlock(x, z, y);
		if (block > 0) return false;
	}

	return true;
}

std::vector<int> Chunk::GetAo(int x, int z, int y, char plane) {
	bool a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;

	if (plane == 'Y') {
		a = CheckIfVoid(x, z - 1, y);
		b = CheckIfVoid(x - 1, z - 1, y);
		c = CheckIfVoid(x - 1, z, y);
		d = CheckIfVoid(x - 1, z + 1, y);
		e = CheckIfVoid(x, z + 1, y);
		f = CheckIfVoid(x + 1, z + 1, y);
		g = CheckIfVoid(x + 1, z, y);
		h = CheckIfVoid(x + 1, z - 1, y);

		std::vector<int> ao = {
		a + b + c,  // TOP LEFT
		g + h + a, // TOP RIGHT
		e + f + g, // BOTTOM RIGHT
		c + d + e, // BOTTOM LEFT 
		};

		return ao;



	}
	else if (plane == 'X') {

		a = CheckIfVoid(x, z, y + 1);
		b = CheckIfVoid(x, z + 1, y + 1);
		c = CheckIfVoid(x, z + 1, y);

		d = CheckIfVoid(x, z + 1, y - 1);
		e = CheckIfVoid(x, z, y - 1);
		f = CheckIfVoid(x, z - 1, y - 1);

		g = CheckIfVoid(x, z - 1, y);
		h = CheckIfVoid(x, z - 1, y + 1);
		//return { 1,1,1,1 };

		/*a = CheckIfVoid(x, y, z - 1);
		b = CheckIfVoid(x, y - 1, z - 1);
		c = CheckIfVoid(x, y - 1, z);

		d = CheckIfVoid(x, y - 1, z + 1);
		e = CheckIfVoid(x, y, z + 1);
		f = CheckIfVoid(x, y + 1, z + 1);

		g = CheckIfVoid(x, y + 1, z);
		h = CheckIfVoid(x, y + 1, z - 1);*/


	}
	else {
		a = CheckIfVoid(x, z, y + 1);
		b = CheckIfVoid(x - 1, z, y + 1);
		c = CheckIfVoid(x - 1, z, y);

		d = CheckIfVoid(x - 1, z, y - 1);
		e = CheckIfVoid(x, z, y - 1);
		f = CheckIfVoid(x + 1, z, y - 1);

		g = CheckIfVoid(x + 1, z, y);
		h = CheckIfVoid(x + 1, z, y + 1);

		/*a = CheckIfVoid(x - 1, y, z);
		b = CheckIfVoid(x - 1, y - 1, z);
		c = CheckIfVoid(x, y - 1, z);

		d = CheckIfVoid(x + 1, y - 1, z);
		e = CheckIfVoid(x + 1, y, z);
		f = CheckIfVoid(x + 1, y + 1, z);

		g = CheckIfVoid(x, y + 1, z);
		h = CheckIfVoid(x - 1, y + 1, z);*/
		//return { 1,1,1,1 };

		std::vector<int> ao = {
		c + d + e, // BOTTOM LEFT 
		a + b + c,  // TOP LEFT
		g + h + a, // TOP RIGHT
		e + f + g, // BOTTOM RIGHT
		};

		return ao;


	}

	std::vector<int> ao = {
		e + f + g, // BOTTOM RIGHT
		g + h + a, // TOP RIGHT
		a + b + c,  // TOP LEFT
		c + d + e, // BOTTOM LEFT 
	};

	return ao;
}


std::vector<glm::vec2> GetBlockUV(BlockFace face, BlockType type) {
	const auto [x, y] = UVs.at(type).at(face);
	return static_cast<TextureAtlas&>(TextureManager::GetTexture("atlas.png")).GetUV(x, y);
}

glm::vec3 colors[4] = {
	{1,0,0},
	{0,1,0},
	{0,0,1},
	{1,1,0}
};


void Chunk::GenerateChunk() {
	//LOG_TRACE("GENERATING CHUNK");

	vertices.clear();
	indices.clear();
	m_CountIndices = 0;
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
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::TOP_FACE, ao);
					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::TOP_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::TOP_FACE, (BlockType)voxelId);


					for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::TOP_FACE,  BlockUV[i], ao[i], colors[i] });
					}

					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 3);

						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 2);
					}
					else {

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 2);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 1);

					}
					m_CountIndices += 4;

				}

				//BOTTOM FACE
				if (CheckIfVoid(x, z, y - 1)) {
					auto ao = GetAo(x, z, y - 1, 'Y');
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::BOTTOM_FACE, ao);
					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::BOTTOM_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::BOTTOM_FACE, (BlockType)voxelId);


					for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BOTTOM_FACE,  BlockUV[i], ao[i], colors[i] });
					}
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices);

						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 3);
					}
					else {

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 3);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);

					}
					m_CountIndices += 4;
				}


				//RIGHT FACE
				if (CheckIfVoid(x + 1, z, y)) {
					auto ao = GetAo(x + 1, z, y, 'X');
					//std::vector<int> ao = { 0,0,0,0 };
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::RIGHT_FACE, ao);
					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::RIGHT_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::RIGHT_FACE, (BlockType)voxelId);


					/*for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::RIGHT_FACE,  BlockUV[i], ao[i], colors[i] });
					}*/

					vertices.push_back(Vertex{ rawVertices[0] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::RIGHT_FACE,  BlockUV[0], ao[3], colors[0] });
					vertices.push_back(Vertex{ rawVertices[1] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::RIGHT_FACE,  BlockUV[1], ao[2], colors[1] });
					vertices.push_back(Vertex{ rawVertices[2] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::RIGHT_FACE,  BlockUV[2], ao[1], colors[2] });
					vertices.push_back(Vertex{ rawVertices[3] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::RIGHT_FACE,  BlockUV[3], ao[0], colors[3] });

					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 3);
					}
					else {
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);

						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);

					}
					m_CountIndices += 4;
				}


				//LEFT FACE
				if (CheckIfVoid(x - 1, z, y)) {
					auto ao = GetAo(x - 1, z, y, 'X');
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::LEFT_FACE, ao);

					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::LEFT_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::LEFT_FACE, (BlockType)voxelId);

					for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::LEFT_FACE,  BlockUV[i], ao[i], colors[i] });
					}
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 3);
					}
					else {
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 1);

						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 2);
					}
					m_CountIndices += 4;
				}


				//BACK FACE
				if (CheckIfVoid(x, z - 1, y)) {
					auto ao = GetAo(x, z - 1, y, 'Z');
					//std::vector<int> ao = { 0,0,0,0 };
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::BACK_FACE, ao);
					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::BACK_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::BACK_FACE, (BlockType)voxelId);


					/*for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BACK_FACE,  BlockUV[i], ao[i], colors[i] });
					}*/

					vertices.push_back(Vertex{ rawVertices[0] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BACK_FACE,  BlockUV[0], ao[3], colors[0] });
					vertices.push_back(Vertex{ rawVertices[1] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BACK_FACE,  BlockUV[1], ao[2], colors[1] });
					vertices.push_back(Vertex{ rawVertices[2] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BACK_FACE,  BlockUV[2], ao[1], colors[2] });
					vertices.push_back(Vertex{ rawVertices[3] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::BACK_FACE,  BlockUV[3], ao[0], colors[3] });

					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 3);
					}
					else {
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 1);

						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices + 2);


					}
					m_CountIndices += 4;
				}

				//FRONT FACE
				if (CheckIfVoid(x, z + 1, y)) {
					auto ao = GetAo(x, z + 1, y, 'Z');
					//GenerateFace(glm::vec3(x, y, z), voxelId, BlockFace::FRONT_FACE, ao);
					std::vector<glm::vec3> rawVertices = rawVertexData.at(BlockFace::FRONT_FACE);
					std::vector<glm::vec2> BlockUV = GetBlockUV(BlockFace::FRONT_FACE, (BlockType)voxelId);


					for (int i = 0; i < rawVertices.size(); i++)
					{
						vertices.push_back(Vertex{ rawVertices[i] + glm::vec3(x, y, z), voxelId, (unsigned int)BlockFace::FRONT_FACE,  BlockUV[i], ao[i], colors[i] });
					}
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 2);

						indices.push_back(m_CountIndices);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 1);
					}
					else {
						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 1);
						indices.push_back(m_CountIndices);

						indices.push_back(m_CountIndices + 3);
						indices.push_back(m_CountIndices + 2);
						indices.push_back(m_CountIndices + 1);
					}
					m_CountIndices += 4;
				}
			}
		}
	}
	//setVAO();
}




void Chunk::GenerateFace(glm::vec3 position, unsigned int voxelId, BlockFace face, std::vector<int> ao) {
	std::vector<glm::vec3> rawVertices = rawVertexData.at(face);
	std::vector<glm::vec2> BlockUV = GetBlockUV(face, (BlockType)voxelId);


	for (int i = 0; i < rawVertices.size(); i++)
	{
		vertices.push_back(Vertex{ rawVertices[i] + position, voxelId, (unsigned int)face,  BlockUV[i], ao[i], colors[i] });
	}

	//TODO check this after texture implementation
	bool flipid = ao[2] + ao[0] > ao[3] + ao[1];
	flipid = false;
	if (flipid) {
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

int Chunk::GetBlock(int x, int z, int y) const {
	return m_Blocks[GetBlockIndex(x, z, y)];
}
void Chunk::SetBlock(int x, int z, int y, unsigned int blockId) {
	if (y > CHUNK_H || y < 0) {
		LOG_ERROR("TEMPORARY FIX");
		return;
	}
	m_Blocks[GetBlockIndex(x, z, y)] = blockId;
}

void Chunk::SetBlock(glm::ivec3 blockPos, unsigned int blockId) {
	if (blockPos.y > CHUNK_H || blockPos.y < 0) {
		LOG_ERROR("TEMPORARY FIX");
		return;
	}
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


