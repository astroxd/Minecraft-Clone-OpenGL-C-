#include "MyChunk.h"

//#include <glm/ext.hpp>


MyChunk::MyChunk() {
	//std::cout << "Chunk created" << std::endl;

	//generateBlocks();
}

MyChunk::MyChunk(ChunkCoord coord, glm::vec3 position, FastNoiseLite* noise) {
	MyChunk::coord = coord;

	MyChunk::position = glm::vec3(position.x * CHUNK_W, position.y * CHUNK_H, position.z * CHUNK_D);

	MyChunk::noise = noise;

	generateBlocks();

}


void MyChunk::setWorldChunks(ChunkUnorderedMap<ChunkCoord, std::unique_ptr<MyChunk>>* worldChunks) {
	MyChunk::worldChunks = worldChunks;

	generateChunk();

}

void MyChunk::generateBlocks() {
	//texture.push_back(Texture("dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));

	int random = rand() % 100 + 1;

	for (size_t x = 0; x < CHUNK_W; x++)
	{
		int wx = x + position.x;

		for (size_t z = 0; z < CHUNK_D; z++)
		{
			int wz = z + position.z;
			int height = 20 + (noise->GetNoise(float(wx), float(wz))) * 20;
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
					blocks[x][z][y] = random;
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
	ChunkUnorderedMap<ChunkCoord, std::unique_ptr<MyChunk>>::iterator neighbourChunk;

	neighbourChunk = worldChunks->find(neighbourChunkCoord);
	if (neighbourChunk == worldChunks->end()) {
		return true;
	}

	int neighbourBlock = neighbourChunk->second->blocks[neighbourBlockX][neighbourBlockZ][y];
	if (neighbourBlock > 0) return false;

	return true;

}



bool MyChunk::checkIfVoid(int x, int z, int y) {
	if (y < 0) return true;
	//LEFT FACE
	if (x == -1) {
		return getNeighbourChunkIndex({ coord.first - 1, coord.second }, CHUNK_W - 1, z, y);
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


std::vector<int> MyChunk::getAo(int x, int z, int y, char plane) {
	//std::cout << "GETGAO" << std::endl;
	bool a, b, c, d, e, f, g, h;

	if (plane == 'Y') {
		a = checkIfVoid(x, z - 1, y);
		b = checkIfVoid(x - 1, z - 1, y);
		c = checkIfVoid(x - 1, z, y);
		d = checkIfVoid(x - 1, z + 1, y);
		e = checkIfVoid(x, z + 1, y);
		f = checkIfVoid(x + 1, z + 1, y);
		g = checkIfVoid(x + 1, z, y);
		h = checkIfVoid(x + 1, z - 1, y);
	}
	else if (plane == 'X') {
		a = checkIfVoid(x, z - 1, y);
		b = checkIfVoid(x, z - 1, y - 1);
		c = checkIfVoid(x, z, y - 1);
		d = checkIfVoid(x, z + 1, y - 1);
		e = checkIfVoid(x, z + 1, y);
		f = checkIfVoid(x, z + 1, y + 1);
		g = checkIfVoid(x, z, y + 1);
		h = checkIfVoid(x, z - 1, y + 1);
	}
	else {
		a = checkIfVoid(x - 1, z, y);
		b = checkIfVoid(x - 1, z, y - 1);
		c = checkIfVoid(x, z, y - 1);
		d = checkIfVoid(x + 1, z, y - 1);
		e = checkIfVoid(x + 1, z, y);
		f = checkIfVoid(x + 1, z, y + 1);
		g = checkIfVoid(x, z, y + 1);
		h = checkIfVoid(x - 1, z, y + 1);
	}

	std::vector<int> ao = {
		a + b + c,
		g + h + a,
		e + f + g,
		c + d + e
	};

	//std::cout << "X: " << x << " Z: " << z << " Y: " << y << " P: " << plane << std::endl;

	/*std::cout << "[0]: " << ao[0] << std::endl;
	std::cout << "[1]: " << ao[1] << std::endl;
	std::cout << "[2]: " << ao[2] << std::endl;
	std::cout << "[3]: " << ao[3] << std::endl;
	*/
	return ao;
}

void MyChunk::generateChunk() {
	std::cout << "GENERATING CHUNK" << std::endl;
	//if (!isVisible) return;
	vertices.clear();
	indices.clear();
	//std::cout << "SIZE: " << worldChunks->size() << std::endl;
	int countIndices = 0;


	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int z = 0; z < CHUNK_D; z++)
		{
			for (int y = 0; y < CHUNK_H; y++)
			{
				unsigned int voxelId = blocks[x][z][y];
				if (voxelId <= 0) continue;


				//TOP FACE
				if (checkIfVoid(x, z, y + 1)) {
					auto ao = getAo(x, z, y + 1, 'Y');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];

					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z), voxelId, 0, glm::vec2(1,0), ao[1] }); //v1
						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z), voxelId, 0,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z + 1), voxelId, 0, glm::vec2(1,1) , ao[3] });	//v3

						//vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z), voxelId, 0, glm::vec2(1,0), ao[1] }); //v1
						//vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z + 1), voxelId, 0, glm::vec2(1,1) , ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z + 1), voxelId, 0, glm::vec2(0,1), ao[2] });	//v2

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;

					}
					else {

						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z), voxelId, 0,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z + 1), voxelId, 0, glm::vec2(1,1) , ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z + 1), voxelId, 0, glm::vec2(0,1), ao[2] });	//v2

						//vertices.push_back(Vertex{ glm::vec3(x    ,y + 1, z), voxelId, 0,  glm::vec2(0,0), ao[0] });	//v0
						//vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z + 1), voxelId, 0, glm::vec2(0,1), ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1,y + 1, z), voxelId, 0, glm::vec2(1,0), ao[1] }); //v1

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;
					}
				}

				//BOTTOM FACE
				if (checkIfVoid(x, z, y - 1)) {
					auto ao = getAo(x, z, y - 1, 'Y');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];

					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z), voxelId, 1, glm::vec2(1,0) , ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x    , y, z + 1), voxelId, 1, glm::vec2(1,1), ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x    , y, z), voxelId, 1,  glm::vec2(0,0), ao[0] });	//v0

						//vertices.push_back(Vertex{ glm::vec3(x + 1, y, z), voxelId, 1, glm::vec2(1,0) , ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z + 1), voxelId, 1, glm::vec2(0,1) , ao[2] });	//v2
						//vertices.push_back(Vertex{ glm::vec3(x    , y, z + 1), voxelId, 1, glm::vec2(1,1), ao[3] });	//v3

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;

					}
					else {


						vertices.push_back(Vertex{ glm::vec3(x    , y, z), voxelId, 1,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z + 1), voxelId, 1, glm::vec2(0,1), ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x    , y, z + 1), voxelId, 1, glm::vec2(1,1), ao[3] });	//v3

						//vertices.push_back(Vertex{ glm::vec3(x    , y, z), voxelId, 1,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y, z), voxelId, 1, glm::vec2(1,0) , ao[1] });	//v1
						//vertices.push_back(Vertex{ glm::vec3(x + 1, y, z + 1), voxelId, 1, glm::vec2(0,1) , ao[2] });	//v2


						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;
					}
				}

				//RIGHT FACE
				if (checkIfVoid(x + 1, z, y)) {

					auto ao = getAo(x + 1, z, y, 'X');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];

					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 2, glm::vec2(1,1), ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 2,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 2, glm::vec2(1,0), ao[1] });	//v1

						//vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 2, glm::vec2(1,1), ao[3] });	//v3
						//vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 2, glm::vec2(1,0), ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 2, glm::vec2(0,1), ao[2] });	//v2

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;

					}
					else {


						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 2,  glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 2, glm::vec2(1,0), ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 2, glm::vec2(0,1), ao[2] });	//v2

						//vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 2,  glm::vec2(0,0), ao[0] });	//v0
						//vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 2, glm::vec2(0,1), ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 2, glm::vec2(1,1), ao[3] });	//v3


						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;
					}
				}

				//LEFT FACE
				if (checkIfVoid(x - 1, z, y)) {

					auto ao = getAo(x - 1, z, y, 'X');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x, y    , z + 1), voxelId, 3, glm::vec2(1,1), ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z), voxelId, 3, glm::vec2(1,0), ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x, y    , z), voxelId, 3, glm::vec2(0,0), ao[0] });	//v0

						//vertices.push_back(Vertex{ glm::vec3(x, y    , z + 1), voxelId, 3, glm::vec2(1,1), ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z + 1), voxelId, 3, glm::vec2(0,1), ao[2] });	//v2
						//vertices.push_back(Vertex{ glm::vec3(x, y + 1, z), voxelId, 3, glm::vec2(1,0), ao[1] });	//v1

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;
					}
					else {

						vertices.push_back(Vertex{ glm::vec3(x, y    , z), voxelId, 3, glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z + 1), voxelId, 3, glm::vec2(0,1), ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x, y + 1, z), voxelId, 3, glm::vec2(1,0), ao[1] });	//v1

						//vertices.push_back(Vertex{ glm::vec3(x, y    , z), voxelId, 3, glm::vec2(0,0), ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x, y    , z + 1), voxelId, 3, glm::vec2(1,1), ao[3] });	//v3
						//vertices.push_back(Vertex{ glm::vec3(x, y + 1, z + 1), voxelId, 3, glm::vec2(0,1), ao[2] });	//v2

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;
					}
				}

				//BACK FACE
				if (checkIfVoid(x, z - 1, y)) {
					auto ao = getAo(x, z - 1, y, 'Z');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 4, glm::vec2(1,1) ,ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x    , y    , z), voxelId, 4,  glm::vec2(0,0) , ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z), voxelId, 4, glm::vec2(1,0) , ao[1] });	//v1

						//vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 4, glm::vec2(1,1) ,ao[3] });	//v3
						//vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z), voxelId, 4, glm::vec2(1,0) , ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 4, glm::vec2(0,1), ao[2] });	//v2

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;

					}
					else {

						vertices.push_back(Vertex{ glm::vec3(x    , y    , z), voxelId, 4,  glm::vec2(0,0) , ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z), voxelId, 4, glm::vec2(1,0) , ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 4, glm::vec2(0,1), ao[2] });	//v2

						//vertices.push_back(Vertex{ glm::vec3(x    , y    , z), voxelId, 4, glm::vec2(0,0) , ao[0] });	//v0
						//vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z), voxelId, 4, glm::vec2(0,1) , ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z), voxelId, 4, glm::vec2(1,1) ,ao[3] });	//v3

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 2);
						indices.push_back(countIndices + 3);
						countIndices += 4;
					}

				}


				//FRONT FACE
				if (checkIfVoid(x, z + 1, y)) {
					auto ao = getAo(x, z + 1, y, 'Z');
					bool flipId = ao[1] + ao[3] > ao[0] + ao[2];
					std::cout << flipId << std::endl;

					if (flipId) {
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 5,glm::vec2(1,1) , ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z + 1), voxelId, 5, glm::vec2(1,0) , ao[1] });	//v1
						vertices.push_back(Vertex{ glm::vec3(x    , y    , z + 1), voxelId, 5,  glm::vec2(0,0) , ao[0] });	//v0

						//vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 5,glm::vec2(1,1) , ao[3] });	//v3
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 5, glm::vec2(0,1) , ao[2] });	//v2
						//vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z + 1), voxelId, 5, glm::vec2(1,0) , ao[1] });	//v1

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;
					}
					else {

						vertices.push_back(Vertex{ glm::vec3(x    , y    , z + 1), voxelId, 5,  glm::vec2(0,0) , ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 5, glm::vec2(0,1) , ao[2] });	//v2
						vertices.push_back(Vertex{ glm::vec3(x    , y + 1, z + 1), voxelId, 5, glm::vec2(1,0) , ao[1] });	//v1

						//vertices.push_back(Vertex{ glm::vec3(x    , y    , z + 1), voxelId, 5,  glm::vec2(0,0) , ao[0] });	//v0
						vertices.push_back(Vertex{ glm::vec3(x + 1, y    , z + 1), voxelId, 5,glm::vec2(1,1) , ao[3] });	//v3
						//vertices.push_back(Vertex{ glm::vec3(x + 1, y + 1, z + 1), voxelId, 5, glm::vec2(0,1) , ao[2] });	//v2

						indices.push_back(countIndices);
						indices.push_back(countIndices + 1);
						indices.push_back(countIndices + 2);

						indices.push_back(countIndices);
						indices.push_back(countIndices + 3);
						indices.push_back(countIndices + 1);
						countIndices += 4;
					}

				}





			}
		}
	}

	//setVAO();
}

void MyChunk::render(Shader& shader, Camera* camera) {
	//if (!camera->isOnFrustum(position)) return;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);


	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//texture[0].texUnit(shader, "tex0", 0);
	//texture[0].Bind();
	Draw(shader);
}
