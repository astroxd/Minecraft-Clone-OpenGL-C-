#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 scale;

out vec4 Color;

void main()
{	
	//gl_Position = camMatrix * model * vec4((aPos - 0.5) * 1.01 + 0.5, 1.0);
	
	gl_Position = camMatrix * model * scale * vec4((aPos), 1.0);
	vec4 color = vec4(0,0,0,1);

	if(aPos.y - (16 * floor(aPos.y/16)) == 0 || aPos.y - (16 * floor(aPos.y/16)) == 1){
		color = vec4(1,1,0,1);
	}
	
	Color = color;
};