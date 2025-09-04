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
	BlockItem(BlockType type = DIRT);
	//BlockItem(BlockType type);

	void SetVAO() override;
	void Draw() override;

	void Transform();
	void UpdateTransformAndScale(const glm::vec3& position, const glm::vec2& scale);

private:
	Shader m_Shader;
	int m_CountIndices = 0;

	BlockType type;

	glm::vec3 position;
	glm::vec2 scale;

	glm::mat4 model, view, proj;


private:
	void GenerateMesh();
	void GenerateFace(BlockType type, BlockFace face);
	void UpdateTransform();

};


#endif 