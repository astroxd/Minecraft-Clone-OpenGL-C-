#include "Shader.h"
#include "Log.h"

#include <glm/gtc/type_ptr.hpp>


std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader() {
	LOG_INFO("Shader Program Created");
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	LOG_INFO("{0} Shader Created", vertexFile);
	Init(vertexFile, fragmentFile);
}

void Shader::Init(const char* vertexFile, const char* fragmentFile) {

	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	LOG_INFO("ID: {0}", ID);
}

void Shader::Activate() {
	glUseProgram(ID);
}
void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER_COMPILATION_ERROR for: {0}", type);
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER_LINKING_ERROR for: {0}", type);
		}
	}
}

void Shader::SetInt(const std::string& u_name, const int value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& u_name, const float value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniform1f(location, value);
}

void Shader::SetBool(const std::string& u_name, const bool value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniform1i(location, value);
}

void Shader::SetMat4(const std::string& u_name, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

}

void Shader::SetVec2(const std::string& u_name, const glm::vec2& value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::SetVec3(const std::string& u_name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(ID, u_name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

