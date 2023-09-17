#version 330 core
layout (location = 0) in vec3 aPos;



uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	gl_Position = camMatrix * model * vec4((aPos - 0.5) * 1.01 + 0.5, 1.0);
};