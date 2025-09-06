#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;

	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);

	void Init(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

	//Set mat4 uniform
	void SetInt(const std::string& u_name, const int value);
	void SetFloat(const std::string& u_name, const float value);
	void SetBool(const std::string& u_name, const bool value);
	void SetMat4(const std::string& u_name, const glm::mat4& value);
	void SetVec2(const std::string& u_name, const glm::vec2& value);
	void SetVec3(const std::string& u_name, const glm::vec3& value);

private:
	void compileErrors(unsigned int shader, const char* type);

};

#endif;

