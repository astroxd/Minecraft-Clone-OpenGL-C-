#include "Mesh.h"
#include "ShaderManager.h"
#include "VoxelHandler.h"


struct VoxelMarkerVertex {
	glm::vec3 pos;
};

class VoxelMarker : public Mesh<VoxelMarkerVertex> {
public:

	VoxelHandler* m_Handler;
	int m_CountIndices = 0;

	VoxelMarker() = default;
	VoxelMarker(VoxelHandler* handler);


	void SetVAO() override;
	void Draw() override;

	void GenerateMesh();
	void GenerateFace(BlockFace face);

	void render(glm::vec3 position);

};