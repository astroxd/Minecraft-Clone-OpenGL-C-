#version 330 core

out vec4 FragColor;
in vec3 voxel_color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = vec4(voxel_color, 1.0);
	//FragColor = texture(tex0, texCoord);
};