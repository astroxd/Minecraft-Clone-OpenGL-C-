#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex1;

uniform vec3 color;

void main()
{	
	vec3 texCol = texture(tex1, texCoord).rgb + color;
	FragColor = vec4(texCol, 1.0);

}