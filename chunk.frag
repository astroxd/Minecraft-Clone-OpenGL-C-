#version 330 core

out vec4 FragColor;

in float shading;
flat in int voxelId;
flat in int faceId;


in vec2 texCoord;
uniform sampler2D tex0;


const vec3 gamma = vec3(2.2);
const vec3 invGamma = 1 / gamma;




void main()
{

vec3 texCol = texture(tex0, texCoord).rgb;
	

	if(voxelId == 2 && faceId == 0){
		//texCol += vec3(0.568, 0.741, 0.349);
		texCol.y = 0.3;
	}


	texCol = pow(texCol, gamma);
	//texCol = texCol * 0.001 + vec3(1);

	

	texCol *= shading; 
	texCol = pow(texCol, invGamma);

	

	FragColor = vec4(texCol, 1.0);
};