#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "Inventory.h"

class Player {

public:
	Player() {
		LOG_INFO("Player Created");
	}
	~Player() {};

	Inventory inventory;

	void Draw() {
		inventory.Draw();
	}

	void Update() {
		inventory.Update();
	}

};

#endif