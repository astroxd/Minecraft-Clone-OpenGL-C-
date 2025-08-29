#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "TextureManager.h"

struct GUIVertex {
	glm::vec3 pos;
	glm::vec2 UV;
};

class GUIMesh : public Mesh<GUIVertex> {
public:

	GUIMesh() {
		LOG_INFO("GUI MESH Created");
		GenerateMesh();
	}

	void GenerateMesh() {
		//std::vector<glm::vec2> UVs = static_cast<TextureAtlas&>(TextureManager::GetTexture("widget.png")).GetUV(0, 9, false, 180.0, 20.0);
		int m_WidthImg = 256;
		int m_HeightImg = 256;

		int x = 0;
		int y = 11;
		float textureWidth = 182.0f;
		float textureHeight = 22.0f;

		std::vector<glm::vec2> FaceUV = {
			glm::vec2((x * textureWidth) / m_WidthImg, (234.0 / m_HeightImg)),
			glm::vec2((x * textureWidth) / m_WidthImg, (256.0 / m_HeightImg)),
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, (256.0 / m_HeightImg)),
			glm::vec2(((x + 1) * textureWidth) / m_WidthImg, (234.0 / m_HeightImg)),

		};


		m_Vertices.push_back(GUIVertex{ glm::vec3(0,0, 0),  FaceUV[0] });
		m_Vertices.push_back(GUIVertex{ glm::vec3(0, 22.0f, 0) , FaceUV[1] });
		m_Vertices.push_back(GUIVertex{ glm::vec3(182.0f,22, 0) , FaceUV[2] });
		m_Vertices.push_back(GUIVertex{ glm::vec3(182.0f ,0, 0) , FaceUV[3] });

		m_Indices.push_back(0);
		m_Indices.push_back(2);
		m_Indices.push_back(1);

		m_Indices.push_back(0);
		m_Indices.push_back(3);
		m_Indices.push_back(2);

		SetVAO();
		Transform();
	}

	void Transform() {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((Window::GetInstance().getWidth() / 2) - 182, 10, 0.0));
		model = glm::scale(model, glm::vec3(2, 2, 1.0));

		glm::mat4 proj = glm::ortho(0.0f, (float)Window::GetInstance().getWidth(), 0.0f, (float)Window::GetInstance().getHeight());

		Shader shader = ShaderManager::GetShader("GUIProgram");
		shader.Activate();
		shader.SetMat4("model", model);
		shader.SetMat4("proj", proj);
	}

	void SetVAO() override {
		VAO.Bind();
		VBO.SetVertices(m_Vertices);
		EBO.SetIndices(m_Indices);

		// Links VBO attributes such as coordinates and colors to VAO
		VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(GUIVertex), (void*)0);
		VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(GUIVertex), (void*)(3 * sizeof(float)));
		// Unbind all to prevent accidentally modifying them
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	};
	void Draw() override {
		Shader shader = ShaderManager::GetShader("GUIProgram");

		shader.Activate();
		//shader.SetMat4("model", model);

		VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	};

	void Update() {
		glm::vec2 xy = Input::getMousePosition();
		Shader shader = ShaderManager::GetShader("GUIProgram");
		shader.Activate();

		if (xy.x >= (Window::GetInstance().getWidth() / 2) - 182 && xy.x <= (Window::GetInstance().getWidth() / 2) + 182) {

			if (xy.y <= 720 - 10 && xy.y >= 720 - 54) {
				shader.SetVec3("color", glm::vec3(1.0, 0.0, 0.0));
				LOG_WARN("INTERSECT");
			}


		}
		else {
			shader.SetVec3("color", glm::vec3(0));
		}
	}
};


class Player {

public:
	Player() {
		LOG_INFO("Player Created");
	}
	~Player() {};


	unsigned int toolbelt[10] = { 0 };
	GUIMesh mesh;

	void Draw() {
		mesh.Draw();
	}

	void Update() {
		mesh.Update();
	}


};

#endif