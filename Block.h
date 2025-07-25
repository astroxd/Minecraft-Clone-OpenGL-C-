#ifndef BLOCK_H 
#define BLOCK_H

#include <unordered_map>
#include <glm/glm.hpp>


enum BlockFace {
	TOP_FACE,
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
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 0.0f, 1.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 1.0f, 1.0f), // TOP RIGHT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP LEFT
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
			glm::vec3(0.0f, 0.0f, 0.0f), // BOTTOM LEFT
			glm::vec3(1.0f, 0.0f, 0.0f), // BOTTOM RIGHT
			glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT
			glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
		}
	},
};

#endif; 
