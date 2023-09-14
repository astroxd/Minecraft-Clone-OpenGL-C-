/*#include "CubeMesh.h";

CubeMesh::CubeMesh(Blocks block) {
	
	std::vector <std::string> faces {
		"grass_side.png",
			"grass_side.png",
			"grass_top.png",
			"dirt.png",
			"grass_side.png",
			"grass_side.png",
	};

	switch (block)
	{
	case CubeMesh::Grass:
		textures.push_back(Texture(faces, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
		Mesh::setData(vertices, indices, textures);
		break;
	case CubeMesh::Cobblestone:
		//Doesn't work, samplerCube requires 6 textures
		textures.push_back(Texture("texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
		Mesh::setData(vertices, indices, textures);
		break;
	default:
		break;
	}

}

/*void CubeMesh::Draw(Shader& shader, Camera& camera)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();
	// Iterating vector by using index

	textures[0].texUnit(shader, "tex0", 0);
	textures[0].Bind();

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}*/