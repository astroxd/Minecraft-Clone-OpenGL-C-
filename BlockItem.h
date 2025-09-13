#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Block.h"

struct BlockItemVertex {
	glm::vec3 pos;
};

struct BlockUVs {
	glm::vec4 UV1;
	glm::vec4 UV2;
	glm::vec4 UV3;
};

struct InstanceItemVertex {
	BlockUVs UVs;
	glm::mat4 view;
};

class BlockItem : public Mesh<BlockItemVertex> {
public:
	BlockItem() = default;
	BlockItem(glm::vec2 scale);

	void SetItems(const std::vector<InventoryItem>& items);
	void SetTransform(const std::vector<glm::vec3>& instancePositions);

	void UpdateTransform(const std::vector<glm::vec3>& instancePositions);

	void Draw() override;

private:
	Shader m_Shader;
	int m_CountIndices;

	glm::vec2 m_Scale{ glm::vec2(1) };

	VBO<InstanceItemVertex> m_InstanceVBO;
	std::vector<InstanceItemVertex> m_InstanceVertices;

	std::vector<InventoryItem> m_Items;

private:
	void SetVAO() override;
	void GenerateMesh();

	void GenerateBlock();
	void GenerateFace(BlockFace face);
	BlockUVs GenerateBlockTexture(const int InventoryIndex);
	glm::mat4 GenerateInstanceView(const glm::vec3& position);

	void Transform();

};


#endif 