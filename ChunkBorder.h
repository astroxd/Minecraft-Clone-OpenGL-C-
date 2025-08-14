#ifndef CHUNK_BORDER_H
#define CHUNK_BORDER_H

#include "Chunk.h"
#include "Utils.h"

class ChunkBorder : public Mesh {
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

	void Render() {
		if (!shouldRender) return;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 scale = glm::mat4(1.0f);
		model = glm::translate(model, ChunkPosition);

		Shader shader = ShaderManager::GetShader("VoxelMarkerProgram");

		shader.Activate();
		shader.SetMat4("model", model);
		shader.SetMat4("scale", scale);

		VAO.Bind();
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);


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
			vertices.push_back(Vertex{ rawVertices[i] + position });
		}

		indices.push_back(m_CountIndices);
		indices.push_back(m_CountIndices + 1);

		indices.push_back(m_CountIndices + 1);
		indices.push_back(m_CountIndices + 2);

		indices.push_back(m_CountIndices + 2);
		indices.push_back(m_CountIndices);

		indices.push_back(m_CountIndices);
		indices.push_back(m_CountIndices + 3);

		indices.push_back(m_CountIndices + 3);
		indices.push_back(m_CountIndices + 2);

		m_CountIndices += 4;
	}

	void GenerateVertices() {
		vertices.clear();
		indices.clear();

		for (int x = 0; x < CHUNK_W; x++) {
			for (int y = 0; y < 50; y++) {
				GenerateFace(BACK_FACE, glm::vec3(x, y, 0.001));
				GenerateFace(FRONT_FACE, glm::vec3(x, y, 14.999));
				GenerateFace(RIGHT_FACE, glm::vec3(14.999, y, x));
				GenerateFace(LEFT_FACE, glm::vec3(0.001, y, x));
			}
		}

		setVAO();
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
