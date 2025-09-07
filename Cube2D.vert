#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aTex1;
layout (location = 2) in vec4 aTex2;
layout (location = 3) in vec4 aTex3;
layout (location = 4) in vec4 aTex4;
layout (location = 5) in vec4 aTex5;
layout (location = 6) in vec4 aTex6;
layout (location = 7) in mat4 aInstanceMatrix;

out vec2 texCoord;

uniform mat4 proj;
uniform mat4 model;

void main()
{	

	gl_Position = proj * aInstanceMatrix * model * vec4(aPos, 1.0);
	//texCoord = aTex[gl_VertexID].xy;

	int face = gl_VertexID / 4;
	int vertexId;
	switch(face){
		case 0:
			vertexId = gl_VertexID % 4;
			switch(vertexId){
				case 0:
					texCoord = aTex1.xy;
					break;
				case 1:
					texCoord = aTex1.zw;
					break;
				case 2:
					texCoord = aTex2.xy;
					break;
				case 3:
					texCoord = aTex2.zw;
					break;
			}
			break;
		case 1:
			vertexId = gl_VertexID % 4;
			switch(vertexId){
				case 0:
					texCoord = aTex3.xy;
					break;
				case 1:
					texCoord = aTex3.zw;
					break;
				case 2:
					texCoord = aTex4.xy;
					break;
				case 3:
					texCoord = aTex4.zw;
					break;
			}
			break;
		case 2:
			vertexId = gl_VertexID % 4;
			switch(vertexId){
				case 0:
					texCoord = aTex5.xy;
					break;
				case 1:
					texCoord = aTex5.zw;
					break;
				case 2:
					texCoord = aTex6.xy;
					break;
				case 3:
					texCoord = aTex6.zw;
					break;
			}
			break;
	
	
	}

}