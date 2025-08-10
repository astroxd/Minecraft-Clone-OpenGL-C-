#include "Mesh.h"
#include "ShaderManager.h"
#include "VoxelHandler.h"


class VoxelMarker : public Mesh {
public:

	VoxelHandler* m_Handler;
	int m_CountIndices = 0;

	VoxelMarker() = default;
	VoxelMarker(VoxelHandler* handler);


	void setVAO() override;

	void GenerateMesh();
	void GenerateFace(BlockFace face);

	void render(glm::vec3 position);

};