#version 330 core
out vec4 FragColor;


in vec2 texCoord;
uniform sampler2D tex2;

void main()
{	
	FragColor = texture(tex2, texCoord);
	//FragColor = vec4(1.0, 0.0,0.0,1.0);
}