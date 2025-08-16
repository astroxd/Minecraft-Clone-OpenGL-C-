#include "TextureManager.h".

std::unordered_map<std::string, std::shared_ptr<Texture>> TextureManager::s_TextureLibrary = {};


void TextureManager::AddTexture(const std::string& name, std::shared_ptr<Texture> texture) {
	if (Exists(name)) return;
	s_TextureLibrary[name] = texture;
}

Texture& TextureManager::GetTexture(const std::string& name) {
	if (Exists(name))
		return *s_TextureLibrary[name];

	LOG_ERROR("{0} Texture doesn't exists", name);
	throw;
}

bool TextureManager::Exists(const std::string& name) {
	auto it = s_TextureLibrary.find(name);
	return it != s_TextureLibrary.end();
}