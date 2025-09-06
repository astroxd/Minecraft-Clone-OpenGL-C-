#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "Hotbar.h"
#include "Inventory.h"

class Player {

public:
	Player() {
		LOG_INFO("Player Created");
	}
	~Player() {};

	Hotbar hotbar;
	Inventory inventory;

	void Draw() {
		hotbar.Draw();
		inventory.Draw();
	}

	void Update() {
		hotbar.Update();
		inventory.Update();
	}

};

#endif