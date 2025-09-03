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


	void GenerateMesh();
	void GenerateFace(BlockType type, BlockFace face);

	void SetVAO() override;
	void Draw() override;

	void Transform();

private:
	Shader m_Shader;
	int m_CountIndices = 0;

};


#endif 