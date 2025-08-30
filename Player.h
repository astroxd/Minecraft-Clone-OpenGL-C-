#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "Hotbar.h"

class Player {

public:
	Player() {
		LOG_INFO("Player Created");
	}
	~Player() {};

	Hotbar hotbar;

	void Draw() {
		hotbar.Draw();
	}

	void Update() {
		hotbar.Update();
	}

};

#endif