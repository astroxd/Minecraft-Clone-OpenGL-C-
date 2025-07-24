#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include "Shader.h"

class ShaderManager {
public:
	// Deleting the copy constructor to prevent copies
	ShaderManager(const ShaderManager& obj) = delete;

	static void AddShader(const std::string& name, std::shared_ptr<Shader> shader);
	static Shader& GetShader(const std::string& name);

private:
	ShaderManager() {};
	static bool Exists(const std::string& name);

private:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_ShaderLibrary;

};

#endif;
