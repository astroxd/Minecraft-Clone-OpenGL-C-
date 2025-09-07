#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Block.h"

struct BlockItemVertex {
	glm::vec3 pos;
	//glm::vec2 UV;
};

struct InstanceItemVertex {
	glm::vec4 UV1;
	glm::vec4 UV2;
	glm::vec4 UV3;
	glm::vec4 UV4;
	glm::vec4 UV5;
	glm::vec4 UV6;
	glm::mat4 view;

};

struct Temp {
	glm::vec3 transform;
	glm::vec4 UV;
	glm::vec4 UV1;
};

class BlockItem : public Mesh<BlockItemVertex> {
public:
	BlockItem();

	void SetVAO() override;
	void Draw() override;

	void SetItems(const std::vector<InventoryItem>& items);
	void SetTransformAndScale(const std::vector<Temp>& instanceTransform,
		const glm::vec2& scale);

	void UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale);

	VBO<InstanceItemVertex> instanceBuffer;
	std::vector <InstanceItemVertex> instanceVertices;


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