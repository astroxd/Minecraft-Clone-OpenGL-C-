#version 330 core

out vec4 FragColor;
in vec3 voxel_color;
in float shading;
uniform sampler2D tex0;


const vec3 gamma = vec3(2.2);
const vec3 invGamma = 1 / gamma;

void main()
{

	vec3 color = voxel_color;

	color *= shading;

	color = pow(color, invGamma);

	FragColor = vec4(color, 1.0);
	//FragColor = texture(tex0, texCoord);
};