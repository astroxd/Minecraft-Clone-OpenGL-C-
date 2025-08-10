#ifndef SCENE_H
#define SCENE_H

#include "World.h"

class Scene {

public:
	World* world;

	Scene() = default;
	Scene(Camera* camera);
	~Scene();

	void setMesh();

	void setCamera(Camera* camera);

	void render();
};


#endif;
