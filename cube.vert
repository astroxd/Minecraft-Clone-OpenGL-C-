#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 scale;

out vec4 Color;

void main()
{	
	//gl_Position = camMatrix * model * vec4((aPos - 0.5) * 1.01 + 0.5, 1.0);
	
	gl_Position = camMatrix * model * scale * vec4((aPos), 1.0);

	Color = vec4(aColor, 1.0);

};