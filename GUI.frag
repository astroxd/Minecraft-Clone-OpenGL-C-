#version 330 core
out vec4 FragColor;

in vec2 texCoord;

in vec4 testColor;

uniform sampler2D tex1;


void main()
{	
	vec4 texCol = texture(tex1, texCoord) + testColor;
	if(texCol.a < 0.1)
		discard;
	FragColor = texCol;

}