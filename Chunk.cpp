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

std::vector<int> Chunk::GetAo(int x, int z, int y, char plane, BlockFace face) {
	bool a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;

	std::vector<int> ao;
	ao.reserve(12);

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
		a = CheckIfVoid(x, z, y + 1);
		b = CheckIfVoid(x, z - 1, y + 1);
		c = CheckIfVoid(x, z - 1, y);

		d = CheckIfVoid(x, z - 1, y - 1);
		e = CheckIfVoid(x, z, y - 1);
		f = CheckIfVoid(x, z + 1, y - 1);

		g = CheckIfVoid(x, z + 1, y);
		h = CheckIfVoid(x, z + 1, y + 1);
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
	}

	ao = {
		c + d + e, // BOTTOM LEFT 
		a + b + c,  // TOP LEFT
		g + h + a, // TOP RIGHT
		e + f + g, // BOTTOM RIGHT
	};


	if (face == BlockFace::RIGHT_FACE || face == BlockFace::BACK_FACE || face == BlockFace::BOTTOM_FACE) {

		ao = {
			e + f + g, // BOTTOM RIGHT
			g + h + a, // TOP RIGHT
			a + b + c,  // TOP LEFT
			c + d + e, // BOTTOM LEFT 
		};
	}

	return ao;
}


std::pair<int, int> Chunk::GetBlockUV(BlockFace face, BlockType type) const {
	//const auto& [x, y] = UVs.at(type).at(face);
	return UVs.at(type).at(face);

	//return static_cast<TextureAtlas&>(TextureManager::GetTexture("atlas.png")).GetUV(x, y);
}





void Chunk::GenerateChunk() {
	//LOG_TRACE("GENERATING CHUNK");

	m_Vertices.clear();
	m_Indices.clear();
	m_CountIndices = 0;
	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int z = 0; z < CHUNK_D; z++)
		{
			for (int y = 0; y < CHUNK_H; y++)
			{
				int voxelId = GetBlock(x, z, y);
				if (voxelId <= 0) continue;


				//TOP FACE
				if (CheckIfVoid(x, z, y + 1)) {
					auto ao = GetAo(x, z, y + 1, 'Y', TOP_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, TOP_FACE, ao);
				}

				//BOTTOM FACE
				if (CheckIfVoid(x, z, y - 1)) {
					auto ao = GetAo(x, z, y - 1, 'Y', BOTTOM_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, BOTTOM_FACE, ao);
				}

				//RIGHT FACE
				if (CheckIfVoid(x + 1, z, y)) {
					auto ao = GetAo(x + 1, z, y, 'X', RIGHT_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, RIGHT_FACE, ao);
				}

				//LEFT FACE
				if (CheckIfVoid(x - 1, z, y)) {
					auto ao = GetAo(x - 1, z, y, 'X', LEFT_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, LEFT_FACE, ao);
				}

				//BACK FACE
				if (CheckIfVoid(x, z - 1, y)) {
					auto ao = GetAo(x, z - 1, y, 'Z', BACK_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, BACK_FACE, ao);
				}

				//FRONT FACE
				if (CheckIfVoid(x, z + 1, y)) {
					auto ao = GetAo(x, z + 1, y, 'Z', FRONT_FACE);
					GenerateFace(glm::vec3(x, y, z), voxelId, FRONT_FACE, ao);

				}
			}
		}
	}
	//setVAO();
}


unsigned int CompressionTest(const glm::vec3& position, const unsigned int voxelId, const BlockFace face,
	const int ao, const int UVx, const int UVy) {

	//pos*		4bit
	//voxelId	6bit
	//face		3bit
	//ao		2bit
	//UV*		4bit

	return (unsigned int(position.x) | unsigned int(position.y) << 4 | unsigned int(position.z) << 8 | voxelId << 12 | face << 18 | ao << 21 | UVx << 23 | UVy << 27);

	/*LOG_INFO(finalNum);
	LOG_INFO(finalNum & 15);
	LOG_INFO((finalNum >> 4) & 15);
	LOG_INFO((finalNum >> 8) & 15);
	LOG_INFO((finalNum >> 12) & 63);
	LOG_INFO((finalNum >> 18) & 7);
	LOG_INFO((finalNum >> 21) & 3);*/


}

void Chunk::GenerateFace(glm::vec3 position, unsigned int voxelId, BlockFace face, std::vector<int> ao) {
	std::vector<glm::vec3> rawVertices = rawVertexData.at(face);
	const auto& [UVx, UVy] = GetBlockUV(face, (BlockType)voxelId);


	for (int i = 0; i < rawVertices.size(); i++)
	{
		unsigned int compressed = CompressionTest(position, voxelId, face, ao[i], UVx, UVy);
		m_Vertices.push_back(Vertex{ compressed });
	}

	//! Higher values means lighter vertex
	bool flipId = ao[1] + ao[3] > ao[0] + ao[2];

	if (flipId) {
		m_Indices.push_back(m_CountIndices + 3);
		m_Indices.push_back(m_CountIndices + 1);
		m_Indices.push_back(m_CountIndices);

		m_Indices.push_back(m_CountIndices + 3);
		m_Indices.push_back(m_CountIndices + 2);
		m_Indices.push_back(m_CountIndices + 1);
	}
	else {
		m_Indices.push_back(m_CountIndices);
		m_Indices.push_back(m_CountIndices + 3);
		m_Indices.push_back(m_CountIndices + 2);

		m_Indices.push_back(m_CountIndices);
		m_Indices.push_back(m_CountIndices + 2);
		m_Indices.push_back(m_CountIndices + 1);
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


void Chunk::SetVAO() {
	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	m_VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(m_VBO, 0, 1, GL_FLOAT, sizeof(Vertex), (void*)0);

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	m_VBO.Unbind();
	EBO.Unbind();
}

void Chunk::Draw() {
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::Render(Camera* camera) {
	//if (!isBuilt) return;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_Position);

	Shader shader = ShaderManager::GetShader("ShaderProgram");

	shader.Activate();
	shader.SetMat4("model", model);
	Draw();
}




