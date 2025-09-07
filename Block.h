#ifndef BLOCK_H 
#define BLOCK_H

#include <unordered_map>
#include <glm/glm.hpp>


enum BlockFace {
	TOP_FACE = 0,
	BOTTOM_FACE,
	RIGHT_FACE,
	LEFT_FACE,
	BACK_FACE,
	FRONT_FACE,
};

//? ALL FACE VERTEXS ARE NOW WRITTEN SUCH AS THE FACE IS LOOKED FROM THE FRONT
//! LearnOpenGL.com
//! When defining your vertex order you visualize the corresponding triangle as if it was facing you, 
//! so each triangle that you're specifying should be counter-clockwise as if you're directly facing that triangle.
const std::unordered_map<BlockFace, std::vector<glm::vec3>> rawVertexData =
{
	{
		BlockFace::TOP_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 1.0f, 1.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT
			glm::vec3(1.0f, 1.0f, 1.0f), // BOTTOM RIGHT
		}
	},
	{
		BlockFace::BOTTOM_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(1.0f, 0.0f, 1.0f), // TOP RIGHT
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM RIGHT
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 0.0f, 1.0f), // TOP LEFT
		}
	},
	{
		BlockFace::RIGHT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM LEFT		
			glm::vec3(1.0f, 1.0f, 1.0f), // TOP LEFT		
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT		
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM RIGHT	
		}
	},
	{
		BlockFace::LEFT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
			glm::vec3(0.0f, 1.0f, 1.0f), // TOP RIGHT
			glm::vec3(0.0f, 0.0f, 1.0f), // BOTTOM RIGHT
		}
	},
	{
		BlockFace::BACK_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP LEFT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP RIGHT
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM RIGHT
		}
	},
	{
		BlockFace::FRONT_FACE,
		std::vector<glm::vec3>
		{
			glm::vec3(0.0f, 0.0f, 1.0f), // BOTTOM LEFT
			glm::vec3(0.0f, 1.0f, 1.0f), // TOP LEFT
			glm::vec3(1.0f, 1.0f, 1.0f), // TOP RIGHT
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM RIGHT
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
				{14,11}
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


struct InventoryItem {
	int id;
	int count;
	int slot;
};

const std::vector<InventoryItem> InventoryItems = {
	InventoryItem(1, 10, 0),
	InventoryItem(2, 1, 2),
	InventoryItem(3, 2, 4),
	//InventoryItem(4, 4, 7),
	//InventoryItem(4, 4, 14),
};



#endif; 
