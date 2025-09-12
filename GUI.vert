#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 proj;
uniform mat4 model;

uniform vec2 slotOffset;
uniform bool highlightSlot;

uniform bool isInventoryOpen = false;


out vec2 texCoord;
out vec4 testColor;

void main()
{	
	vec4 pos = vec4(aPos, 1.0);
	vec4 color = vec4(0.0,0.0,0.0, 0.0);
	
	if(isInventoryOpen){
		if(gl_VertexID >= 0 && gl_VertexID < 4){
			pos += vec4(slotOffset, 0.0, 0.0);
			if(!highlightSlot)
				pos.w = 0;
			color = vec4(0.0,0.0,0.0, 0.2);
		}
	}
	else{
		if(gl_VertexID >= 0 && gl_VertexID < 4){
			pos += vec4(slotOffset, 0.0, 0.0);
		}
	}


	gl_Position = proj * model * pos;

	texCoord = aTex;
	testColor = color;
}