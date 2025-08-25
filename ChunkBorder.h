#ifndef CHUNK_BORDER_H
#define CHUNK_BORDER_H

#include "Chunk.h"
#include "Utils.h"

struct ChunkBorderVertex {
	glm::vec3 pos;
};

class ChunkBorder : public Mesh<ChunkBorderVertex> {
public:

	ChunkBorder() {
		LOG_INFO("CHUNK BORDER CREATED");
	};

	void Init(Camera* camera) {
		this->camera = camera;
		GenerateVertices();
	}

	void Update() {

		int cx = floor(camera->Position.x / CHUNK_W);
		int cz = floor(camera->Position.z / CHUNK_D);
		ChunkPosition = glm::vec3(cx * CHUNK_W, 0, cz * CHUNK_D);
		Input();

	}

	void SetVAO() override {
		VAO.Bind();
		VBO.SetVertices(m_Vertices);
		EBO.SetIndices(m_Indices);

		// Links VBO attributes such as coordinates and colors to VAO
		VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(ChunkBorderVertex), (void*)0);
		// Unbind all to prevent accidentally modifying them
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
	void Draw() override {
		VAO.Bind();

		glDrawElements(GL_LINES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	}


	void Render() {
		if (!shouldRender) return;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 scale = glm::mat4(1.0f);
		model = glm::translate(model, ChunkPosition);

		Shader shader = ShaderManager::GetShader("VoxelMarkerProgram");

		shader.Activate();
		shader.SetMat4("model", model);
		shader.SetMat4("scale", scale);

		Draw();
	}
private:
	Camera* camera;
	glm::vec3 ChunkPosition;
	int m_CountIndices = 0;
	bool shouldRender = false;
	std::chrono::milliseconds m_LastButton;

private:
	void GenerateFace(BlockFace face, const glm::vec3& position) {
		std::vector<glm::vec3> rawVertices = rawVertexData.at(face);

		for (int i = 0; i < rawVertices.size(); i++)
		{
			m_Vertices.push_back(ChunkBorderVertex{ rawVertices[i] + position });
		}

		m_Indices.push_back(m_CountIndices);
		m_Indices.push_back(m_CountIndices + 1);

		m_Indices.push_back(m_CountIndices + 1);
		m_Indices.push_back(m_CountIndices + 2);

		m_Indices.push_back(m_CountIndices + 2);
		m_Indices.push_back(m_CountIndices + 3);

		m_Indices.push_back(m_CountIndices + 3);
		m_Indices.push_back(m_CountIndices);

		m_CountIndices += 4;
	}

	void GenerateVertices() {
		m_Vertices.clear();
		m_Indices.clear();

		for (int x = 0; x < CHUNK_W; x++) {
			for (int y = 0; y < 50; y++) {
				GenerateFace(BACK_FACE, glm::vec3(x, y, 0.001));
				GenerateFace(FRONT_FACE, glm::vec3(x, y, 14.999));
				GenerateFace(RIGHT_FACE, glm::vec3(14.999, y, x));
				GenerateFace(LEFT_FACE, glm::vec3(0.001, y, x));
			}
		}

		SetVAO();
	}

	void Input() {

		if (Input::isKeyPressed(Key::F5)) {
			std::chrono::milliseconds time = Utils::GetMs();
			if ((time - m_LastButton).count() > 100) {
				shouldRender = !shouldRender;
			}
			m_LastButton = time;
		}
	}
};

#endif
