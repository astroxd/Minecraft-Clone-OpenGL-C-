#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

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

	Shader() { std::cout << "Shader Program Created" << std::endl; };
	Shader(const char* vertexFile, const char* fragmentFile);

	void init(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
	void compileErrors(unsigned int shader, const char* type);

	//Set mat4 uniform
	void SetMat4(const std::string& u_name, const glm::mat4& value);
};

#endif;

