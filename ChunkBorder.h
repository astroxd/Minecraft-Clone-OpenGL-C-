#ifndef CHUNK_BORDER_H
#define CHUNK_BORDER_H

#include "Chunk.h"
#include "Utils.h"

struct ChunkBorderVertex {
	glm::vec3 pos;
	glm::vec3 color;
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
		VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(ChunkBorderVertex), (void*)(3 * sizeof(float)));
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

		glLineWidth(1.2);
		Draw();
	}
private:
	Camera* camera;
	glm::vec3 ChunkPosition;
	int m_CountIndices = 0;
	bool shouldRender = false;
	std::chrono::milliseconds m_LastButton;

	const int MAX_Y = 64;

private:
	void GenerateLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0)) {

		m_Vertices.push_back(ChunkBorderVertex{ begin, color });
		m_Vertices.push_back(ChunkBorderVertex{ end, color });

		m_Indices.push_back(m_CountIndices);
		m_Indices.push_back(m_CountIndices + 1);

		m_CountIndices += 2;
	}

	void GenerateVertices() {
		m_Vertices.clear();
		m_Indices.clear();

		for (int x = 0; x <= CHUNK_W; x += 2) {
			double xx = x;
			if (x == 0) xx = 0.001;
			if (x == CHUNK_W) xx = 15.999;

			glm::vec3 color = glm::vec3(1.0, 1.0, 0.0);

			if ((x / 2) % 2 == 0) color = glm::vec3(0.0, 1.0, 1.0);

			if (x == 0 || x == CHUNK_W) color = glm::vec3(0.0, 0.0, 1.0);

			GenerateLine(glm::vec3(xx, 0, 0.001), glm::vec3(xx, MAX_Y, 0.001), color);		//BACKFACE
			GenerateLine(glm::vec3(xx, 0, 15.999), glm::vec3(xx, MAX_Y, 15.999), color);	//FRONTFACE
			GenerateLine(glm::vec3(15.999, 0, xx), glm::vec3(15.999, MAX_Y, xx), color);	//RIGHTFACE
			GenerateLine(glm::vec3(0.001, 0, xx), glm::vec3(0.001, MAX_Y, xx), color);		//LEFTFACE

		}

		for (int y = 0; y <= MAX_Y; y += 2) {
			glm::vec3 color = glm::vec3(1.0, 1.0, 0.0);

			if (y % 8 == 0) color = glm::vec3(0.0, 1.0, 1.0);
			if (y % 16 == 0) color = glm::vec3(0.0, 0.0, 1.0);

			GenerateLine(glm::vec3(0.001, y, 0.001), glm::vec3(15.999, y, 0.001), color);
			GenerateLine(glm::vec3(0.001, y, 15.999), glm::vec3(15.999, y, 15.999), color);
			GenerateLine(glm::vec3(0.001, y, 0.001), glm::vec3(0.001, y, 15.999), color);
			GenerateLine(glm::vec3(15.999, y, 0.001), glm::vec3(15.999, y, 15.999), color);
		}

		GenerateLine(glm::vec3(CHUNK_W / 2, 0.0, CHUNK_D / 2), glm::vec3(CHUNK_W / 2, MAX_Y, CHUNK_D / 2), glm::vec3(1.0, 0.0, 0.0));

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
