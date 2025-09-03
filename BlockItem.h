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
	BlockItem(glm::vec3 position);

	void SetVAO() override;
	void Draw() override;

	void Transform();

private:
	Shader m_Shader;
	int m_CountIndices = 0;

	glm::vec3 position;

	glm::mat4 model, view, proj;


private:
	void GenerateMesh();
	void GenerateFace(BlockType type, BlockFace face);
	void UpdateTransform();

};


#endif 