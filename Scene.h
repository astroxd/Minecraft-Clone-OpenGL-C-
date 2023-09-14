#ifndef SCENE_H
#define SCENE_H

//#include "shaderClass.h"
//#include "MyChunk.h"
#include "World.h"

class Scene {

	public:
		World world;

		GLFWwindow* window;
		GLFWwindow* second_context;

		Scene();

		void setMesh();

		void setShader(Shader& shader);
		void setCamera(Camera* camera);

		void render(Shader& shader);
};


#endif;
