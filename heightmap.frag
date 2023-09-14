#version 330 core

out vec4 FragColor;

in float Height;

in vec3 color;
in vec2 texCoord;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    FragColor = vec4(0.0, h, 0.0, 1.0);
}