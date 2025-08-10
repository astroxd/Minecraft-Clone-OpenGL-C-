#version 330 core

out vec4 FragColor;

in float shading;
flat in int voxelId;

in vec2 texCoord;
uniform sampler2D tex0;


const vec3 gamma = vec3(2.2);
const vec3 invGamma = 1 / gamma;

void main()
{

	vec3 texCol = texture(tex0, texCoord).rgb;
	texCol = pow(texCol, gamma);
	texCol *= shading; 
	texCol = pow(texCol, invGamma);

	FragColor = vec4(texCol, 1.0);
};