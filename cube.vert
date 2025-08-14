#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 scale;

void main()
{	
	//gl_Position = camMatrix * model * vec4((aPos - 0.5) * 1.01 + 0.5, 1.0);
	
	 gl_Position = camMatrix * model * scale * vec4((aPos), 1.0);
};