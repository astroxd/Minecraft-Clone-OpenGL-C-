#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Block.h"

struct BlockItemVertex {
	glm::vec3 pos;
	glm::vec2 UV;
};

class BlockItem : public Mesh<BlockItemVertex> {
public:
	BlockItem();

	void SetVAO() override;
	void Draw() override;

	void SetItems(const std::vector<InventoryItem>& items);
	void SetTransformAndScale(const glm::vec3& position, const glm::vec2& scale);

	void UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale);

private:
	Shader m_Shader;
	int m_CountIndices;

	glm::vec3 m_Position;
	glm::vec2 m_Scale;

	std::vector<InventoryItem> m_Items;

private:
	void GenerateMesh();
	void GenerateBlock(BlockType type, const int slot);
	void GenerateFace(BlockType type, BlockFace face, const int slot);
	glm::vec3 TransformVertex(const glm::vec3& vertex, const int slot);

	void Transform();

};


#endif 