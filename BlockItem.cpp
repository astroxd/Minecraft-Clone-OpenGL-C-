#include "BlockItem.h"
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>

#include "Block.h"
#include "Log.h"
#include "TextureManager.h"
#include "TextureAtlas.h"

BlockItem::BlockItem() {
	LOG_INFO("BlockItem Created");
	m_Shader = ShaderManager::GetShader("Cube2DProgram");
	GenerateMesh();
}

void BlockItem::GenerateMesh() {
	std::vector<glm::vec3> vertices = rawVertexData.at(FRONT_FACE);
	std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(14, 14);
	int countIndices = 0;




	/*m_Vertices.push_back(BlockItemVertex{ vertices[0] + glm::vec3(200.0f,100.0f, 0) });
	m_Vertices.push_back(BlockItemVertex{ vertices[1] + glm::vec3(200.0f,200.0f, 0) });
	m_Vertices.push_back(BlockItemVertex{ vertices[2] + glm::vec3(300.0f,200.0f, 0) });
	m_Vertices.push_back(BlockItemVertex{ vertices[3] + glm::vec3(300.0f,100.0f, 0) });*/

	/*m_Vertices.push_back(BlockItemVertex{ glm::vec3(100, 50, 1) });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(100, 150, 1) });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(200, 150, 1) });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(200, 50, 1) });*/

	/*m_Vertices.push_back(BlockItemVertex{ glm::vec3(-0.5f, -0.5f, 0.0f), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(-0.5f,  0.5f, 0.0f), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0.5f,  0.5f, 0.0f), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0.5f, -0.5f, 0.0f),  UVs[3] });*/

	//! PERSPECTIVE
	/*m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 0, 0), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 1, 0), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 0), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 0),  UVs[3] });*/

	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 0, 0), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 1, 0), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 0), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 0),  UVs[3] });





	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);

	countIndices += 4;


	//vertices = rawVertexData.at(RIGHT_FACE);
	//m_Vertices.push_back(BlockItemVertex{ vertices[0] + glm::vec3(200.0f,100.0f, 0) });
	//m_Vertices.push_back(BlockItemVertex{ vertices[1] + glm::vec3(200.0f,200.0f, 0) });
	//m_Vertices.push_back(BlockItemVertex{ vertices[2] + glm::vec3(300.0f,200.0f, 0) });
	//m_Vertices.push_back(BlockItemVertex{ vertices[3] + glm::vec3(300.0f,100.0f, 0) });


	//m_Indices.push_back(countIndices);
	//m_Indices.push_back(countIndices + 3);
	//m_Indices.push_back(countIndices + 2);

	//m_Indices.push_back(countIndices);
	//m_Indices.push_back(countIndices + 2);
	//m_Indices.push_back(countIndices + 1);


	/*m_Vertices.push_back(BlockItemVertex{ glm::vec3(21, 0, 1), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(21,  21, 1), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(21,  21, 21), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(21, 0, 21), UVs[3] });*/



	//! PERSPECTIVE
	/*m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 0), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 0), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 2), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 2),  UVs[3] });*/


	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 0), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 0), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 1), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 0, 1),  UVs[3] });


	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);



	countIndices += 4;
	UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("GUIatlas.png")).GetUV(14, 11);

	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 1, 0), UVs[0] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(0, 1, 1), UVs[1] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 1), UVs[2] });
	m_Vertices.push_back(BlockItemVertex{ glm::vec3(1, 1, 0),  UVs[3] });

	//glm::vec3(0.0f, 1.0f, 1.0f), // BOTTOM LEFT
	//	glm::vec3(0.0f, 1.0f, 0.0f), // TOP LEFT
	//	glm::vec3(1.0f, 1.0f, 0.0f), // TOP RIGHT
	//	glm::vec3(1.0f, 1.0f, 1.0f), // BOTTOM RIGHT

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 3);
	m_Indices.push_back(countIndices + 2);

	m_Indices.push_back(countIndices);
	m_Indices.push_back(countIndices + 2);
	m_Indices.push_back(countIndices + 1);



	countIndices += 4;


	SetVAO();
	Transform();

}

void BlockItem::SetVAO()
{
	VAO.Bind();
	VBO.SetVertices(m_Vertices);
	EBO.SetIndices(m_Indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(BlockItemVertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(BlockItemVertex), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void BlockItem::Draw()
{
	m_Shader.Activate();

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(100, 0, 0));
	model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(260.0f));

	m_Shader.SetMat4("model", model);

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}

void BlockItem::Transform() {
	float size = 160.0f;
	auto m_WindowSize = Window::GetInstance().getWindowSize();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(size));


	glm::mat4 view = glm::mat4(1.0f);


	//? First attempt
	//float zOffset = ((0.7 * size) - (size / 2)) + 0.15 * size;
	//view = glm::translate(view, glm::vec3(510, 200, -zOffset));
	//glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.1f, 1.4f * size);


	float zOffset = 1.4f * size;
	view = glm::translate(view, glm::vec3(510, 200, -zOffset));

	glm::mat4 proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, 0.0f, 2.5f * zOffset);

	m_Shader.Activate();
	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("proj", proj);
	m_Shader.SetMat4("view", view);
}
