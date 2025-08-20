#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in unsigned int aVoxelId;
layout (location = 2) in unsigned int aFaceId;
layout (location = 3) in vec2 aTex;
layout (location = 4) in int aoId;
layout (location = 5) in vec3 aColor;

out float shading;
flat out int voxelId;
flat out int faceId;

out vec3 color;


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



void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
    
    shading = faceShading[aFaceId] * aoValues[aoId];
    
    texCoord = vec2(aTex.x, aTex.y);
    voxelId = int(aVoxelId);
    faceId = int(aFaceId);

    color = aColor;

};