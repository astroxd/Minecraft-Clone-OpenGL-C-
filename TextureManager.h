#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>

#include "Texture.h"
#include "Log.h"

//If I need another manager, create a pure virtual class "Resource Manager"

class TextureManager {
public:
	// Deleting the copy constructor to prevent copies
	TextureManager(const TextureManager& obj) = delete;

	static void AddTexture(const std::string& name, std::shared_ptr<Texture> texture);
	static Texture& GetTexture(const std::string& name);

private:
	TextureManager();

	static bool Exists(const std::string& name);

private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> s_TextureLibrary;
};

#endif 
