#ifndef BLOCK_H 
#define BLOCK_H

#include <unordered_map>
#include <glm/glm.hpp>


enum BlockFace {
	TOP_FACE = 0,
	BOTTOM_FACE,
	RIGHT_FACE,
	LEFT_FACE,
	FRONT_FACE,
	BACK_FACE
};


const std::unordered_map<BlockFace, std::vector<glm::vec3>> rawVertexData =
{
	{
		BlockFace::TOP_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 1.0f, 1.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 1.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
		}
	},
	{
		BlockFace::BOTTOM_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 0.0f, 1.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 0.0f, 0.0f), // TOP RIGHT
			glm::vec3(0.0f, 0.0f, 0.0f), // TOP LEFT
		}
	},
	{
		BlockFace::RIGHT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP LEFT
			glm::vec3(1.0f, 1.0f, 1.0f), // TOP RIGHT
		}
	},
	{
		BlockFace::LEFT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 0.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(0.0f, 1.0f, 1.0f), // TOP RIGHT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
		}
	},
	{
		BlockFace::FRONT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 0.0f, 1.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 1.0f, 1.0f), // TOP RIGHT
			glm::vec3(0.0f, 1.0f, 1.0f), // TOP LEFT
		}
	},
	{
		BlockFace::BACK_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM RIGHT
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT
		}
	},
};


enum BlockType {
	AIR = 0,
	DIRT,
	GRASS,
	COBBLESTONE,
	BEDROCK,
};
const std::unordered_map<BlockType, std::unordered_map<BlockFace, std::pair<int, int>>> UVs =
{
	{
		DIRT,
		{
			{
				TOP_FACE,
				{5,9},
			},
			{
				BOTTOM_FACE,
				{5,9},
			},
			{
				RIGHT_FACE,
				{5,9},
			},
			{
				LEFT_FACE,
				{5,9},
			},
			{
				FRONT_FACE,
				{5,9},
			},
			{
				BACK_FACE,
				{5,9},
			},
		}
	},
	{
		GRASS,
		{
			{
				TOP_FACE,
				{14,11}
			},
			{
				BOTTOM_FACE,
				{5,9}
			},
			{
				RIGHT_FACE,
				{14,14}
			},
			{
				LEFT_FACE,
				{14,14}
			},
			{
				FRONT_FACE,
				{14,14}
			},
			{
				BACK_FACE,
				{14,14}
			},
		}
	},
	{
		COBBLESTONE,
		{
			{
				TOP_FACE,
				{8,14}
			},
			{
				BOTTOM_FACE,
				{8,14}
			},
			{
				RIGHT_FACE,
				{8,14}
			},
			{
				LEFT_FACE,
				{8,14}
			},
			{
				FRONT_FACE,
				{8,14}
			},
			{
				BACK_FACE,
				{8,14}
			},
		}
	},
	{
		BEDROCK,
		{
			{
				TOP_FACE,
				{4,13}
			},
			{
				BOTTOM_FACE,
				{4,13}
			},
			{
				RIGHT_FACE,
				{4,13}
			},
			{
				LEFT_FACE,
				{4,13}
			},
			{
				FRONT_FACE,
				{4,13}
			},
			{
				BACK_FACE,
				{4,13}
			},
		}
	}

};

//const glm::vec2 BlockUV[4] = {
//	glm::vec2((x * textureWidth) / atlasWidth, (y * textureWidth) / atlasHeight),
//	glm::vec2(((x + 1) * textureWidth) / atlasWidth, (y * textureWidth) / atlasHeight),
//	glm::vec2(((x + 1) * textureWidth) / atlasWidth, ((y + 1) * textureWidth) / atlasHeight),
//	glm::vec2((x * textureWidth) / atlasWidth, ((y + 1) * textureWidth) / atlasHeight),
//
//	//glm::vec2(0,0), // BOTTOM LEFT
//	//glm::vec2(1,0), // BOTTOM RIGHT
//	//glm::vec2(1,1), // TOP RIGHT
//	//glm::vec2(0,1)  // TOP LEFT
//};

#endif; 
