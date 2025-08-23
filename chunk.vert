#version 460 core

struct VertexData{
	uint compressed;
	uint compressedUV;
};

layout(std430, binding = 0) readonly buffer vertexBuffer{
	VertexData packedMeshData[];
};

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


int indices[6] = {0, 3, 2, 0, 2, 1};
int flippedIndices[6] = {3, 1, 0, 3, 2, 1};

void main()
{
	//The division is truncated to the nearest integer so the 
	//6 consecutives IDs referes all to the same index/face
	const int index = gl_VertexID / 6;
	const uint packedData = packedMeshData[index].compressed;
	const uint packedUV = packedMeshData[index].compressedUV;
	const int currVertexID = gl_VertexID % 6;


    uint x = (packedData) & 15u;
    uint y = (packedData >> 4) & 15u;
    uint z = (packedData >> 8) & 15u;
    uint aVoxelId = (packedData >> 12) & 63u;
    uint aFaceId = (packedData >> 18) & 7u;
    
	uint ao0 = (packedData >> 21) & 3u;
	uint ao1 = (packedData >> 23) & 3u;
	uint ao2 = (packedData >> 25) & 3u;
	uint ao3 = (packedData >> 27) & 3u;
	uint ao[4] = {ao0, ao1, ao2, ao3};

	bool flipId = bool((packedData >> 29) & 1u);
	if(flipId == true){
		indices = flippedIndices;
	}

	int UVx = int((packedUV) & 63u);
	int UVy = int((packedUV >> 6) & 63u);

	vec3 position = vec3(x,y,z);
	vec3 vertexPosition;

	switch(aFaceId){
		case 0u:
			vertexPosition = topFacePositions[indices[currVertexID]] + position;
			break;
		case 1u:
			vertexPosition = bottomFacePositions[indices[currVertexID]] + position;
			break;
		case 2u:
			vertexPosition = rightFacePositions[indices[currVertexID]] + position;
			break;
		case 3u:
			vertexPosition = leftFacePositions[indices[currVertexID]] + position;
			break;
		case 4u:
			vertexPosition = backFacePositions[indices[currVertexID]] + position;
			break;
		case 5u:
			vertexPosition = frontFacePositions[indices[currVertexID]] + position;
			break;
	}

	gl_Position = camMatrix * model * vec4(vertexPosition, 1.0);
    
    shading = faceShading[faceId] * aoValues[ao[indices[currVertexID]]];
    

	vec2 textureUV; 

	UVy = UVy + 15;
	switch(indices[currVertexID]){
		case 0:
			 textureUV = vec2((UVx*widthImg)/textureWidth, (UVy*widthImg)/textureHeight); 
			break;
		case 1:
			textureUV = vec2((UVx*widthImg)/textureWidth, ((UVy+1)*widthImg)/textureHeight); 
			break;
		case 2:
			textureUV = vec2(((UVx+1)*widthImg)/textureWidth, ((UVy+1)*widthImg)/textureHeight); 
			break;
		case 3:
			textureUV = vec2(((UVx+1)*widthImg)/textureWidth, (UVy*widthImg)/textureHeight); 
			break;
	}

    texCoord = textureUV;
    voxelId = int(aVoxelId);
    faceId = int(aFaceId);
}