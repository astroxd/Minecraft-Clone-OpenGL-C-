#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aTopBottomTex;
layout (location = 2) in vec4 aRightLeftTex;
layout (location = 3) in vec4 aFrontBackTex;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 texCoord;

uniform mat4 proj;
uniform mat4 model;


const float deltaX = 0.03125;
const float deltaY = 0.03125;


vec2 texUV[6] = vec2[6](
	aTopBottomTex.xy, aTopBottomTex.zw,
	aRightLeftTex.xy, aRightLeftTex.zw,
	aFrontBackTex.xy, aFrontBackTex.zw
);

void setTexCoord(int face){
	texCoord = texUV[face];

	int vertexId = gl_VertexID % 4;
	switch(vertexId){
		case 0:
			//texCoord = aTex1.xy;
			break;
		case 1:
			texCoord.y += deltaY;
			break;
		case 2:
			texCoord += vec2(deltaX, deltaY);
			break;
		case 3:
			texCoord.x += deltaX;
			break;
	}
}

void main()
{	
	gl_Position = proj * aInstanceMatrix * model * vec4(aPos, 1.0);
	setTexCoord(gl_VertexID / 4);

}