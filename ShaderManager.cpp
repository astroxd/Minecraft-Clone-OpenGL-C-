#include "ShaderManager.h"
#include "Log.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::s_ShaderLibrary = {};

void ShaderManager::AddShader(const std::string& name, std::shared_ptr<Shader> shader) {
	if (Exists(name)) return;

	s_ShaderLibrary[name] = shader;
};

Shader& ShaderManager::GetShader(const std::string& name) {
	if (Exists(name))
		return *s_ShaderLibrary[name];

	LOG_ERROR("{0} Shader doesn't exists", name);
	throw;
}

bool ShaderManager::Exists(const std::string& name)
{
	auto it = s_ShaderLibrary.find(name);
	return it != s_ShaderLibrary.end();
}