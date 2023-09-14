#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in unsigned int voxelId;
layout (location = 2) in unsigned int faceId;
layout (location = 3) in vec2 aTex;

out vec3 voxel_color;
out vec2 texCoord;
uniform mat4 camMatrix;
uniform mat4 model;


vec3 hash31(float p) {
    vec3 p3 = fract(vec3(p * 21.2) * vec3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xxy + p3.yzz) * p3.zyx) + 0.05;
}

void main()
{
	//gl_Position = camMatrix * model * vec4(aPos, 1.0);
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
    voxel_color = hash31(voxelId);
    //texCoord = aTex;
};