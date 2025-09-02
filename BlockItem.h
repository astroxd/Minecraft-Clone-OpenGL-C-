#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <glm/glm.hpp>

#include "Mesh.h"


struct BlockItemVertex {
	glm::vec3 pos;
	glm::vec2 UV;
};

class BlockItem : public Mesh<BlockItemVertex> {
public:
	BlockItem();


	void GenerateMesh();

	void SetVAO() override;
	void Draw() override;

	void Transform();

private:
	Shader m_Shader;

};


#endif 