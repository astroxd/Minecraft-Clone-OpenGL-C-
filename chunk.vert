#version 330 core
layout (location = 0) in uint aCompressed;

out float shading;
flat out int voxelId;
flat out int faceId;


out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;

const float aoValues[4] = float[4](0.1, 0.25, 0.5, 1.0);

const float faceShading[6] = float[6](
    1.0, 0.5,  // top bottom
    0.5, 0.8,  // right left
    0.5, 0.8   // back front
);

vec3 hash31(float p) {
    vec3 p3 = fract(vec3(p * 21.2) * vec3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xxy + p3.yzz) * p3.zyx) + 0.05;
}

vec3 backFacePositions[4] = vec3[4](
	vec3(1.0f, 0.0f, 0.0f), // Top Left
	vec3(1.0f, 1.0f, 0.0f), // Top Right
	vec3(0.0f, 1.0f, 0.0f),  // Bottom Right
	vec3(0.0f, 0.0f, 0.0f) // Bottom left
);

vec3 frontFacePositions[4] = vec3[4](
	vec3(0.0f, 0.0f, 1.0f), // Bottom left
	vec3(0.0f, 1.0f, 1.0f), // Top Left
	vec3(1.0f, 1.0f, 1.0f), // Top Right
	vec3(1.0f, 0.0f, 1.0f)  // Bottom Right
);

vec3 leftFacePositions[4] = vec3[4](
	vec3(0.0f, 0.0f, 0.0f), // Bottom left
	vec3(0.0f, 1.0f, 0.0f),  // Bottom right
	vec3(0.0f, 1.0f, 1.0f), // Top right
	vec3(0.0f, 0.0f, 1.0f) // Top left
);

vec3 rightFacePositions[4] = vec3[4](
	vec3(1.0f, 0.0f, 1.0f),  // Top left
	vec3(1.0f, 1.0f, 1.0f), // Top right
	vec3(1.0f, 1.0f, 0.0f), // Bottom right
	vec3(1.0f, 0.0f, 0.0f) // Bottom left
);

vec3 topFacePositions[4] = vec3[4](
	vec3(0.0f, 1.0f, 1.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 1.0f)
);

vec3 bottomFacePositions[4] = vec3[4](
	vec3(1.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f)
);


const float textureWidth = 512.0;
const float textureHeight = 512.0;
const float widthImg = 16.0;



void main()
{
    uint x = (aCompressed) & 15u;
    uint y = (aCompressed >> 4) & 15u;
    uint z = (aCompressed >> 8) & 15u;
    uint aVoxelId = (aCompressed >> 12) & 63u;
    uint aFaceId = (aCompressed >> 18) & 7u;
    uint ao = (aCompressed >> 21) & 3u;
	int UVx = int((aCompressed >> 23) & 15u);
	int UVy = int((aCompressed >> 27) & 15u);

	vec3 position = vec3(x,y,z);


	int vertexID = int(mod(gl_VertexID, 4.0f));
	vec3 vertexPosition;

	switch(aFaceId){
		case 0u:
			vertexPosition = topFacePositions[vertexID] + position;
			break;
		case 1u:
			vertexPosition = bottomFacePositions[vertexID] + position;
			break;
		case 2u:
			vertexPosition = rightFacePositions[vertexID] + position;
			break;
		case 3u:
			vertexPosition = leftFacePositions[vertexID] + position;
			break;
		case 4u:
			vertexPosition = backFacePositions[vertexID] + position;
			break;
		case 5u:
			vertexPosition = frontFacePositions[vertexID] + position;
			break;
	}

	gl_Position = camMatrix * model * vec4(vertexPosition, 1.0);
    
    shading = faceShading[faceId] * aoValues[ao];
    

	vec2 textureUV; 
	//= vec2(14*16/512.0, 29*16/512.0); 
	

	UVy = UVy + 15;
	switch(vertexID){
		case 0:
			 textureUV = vec2((UVx*widthImg)/textureWidth, (UVy*widthImg)/textureHeight); 
			break;
		case 1:
			textureUV = vec2((UVx*widthImg)/textureWidth, ((UVy+1)*widthImg)/textureHeight); 
			//textureUV.y += 0.03125; 
			break;
		case 2:
			textureUV = vec2(((UVx+1)*widthImg)/textureWidth, ((UVy+1)*widthImg)/textureHeight); 
			//textureUV += vec2(0.03125);
			break;
		case 3:
			textureUV = vec2(((UVx+1)*widthImg)/textureWidth, (UVy*widthImg)/textureHeight); 
			//textureUV.x += 0.03125; 
			break;
	}

    texCoord = textureUV;
    voxelId = int(aVoxelId);
    faceId = int(aFaceId);
}