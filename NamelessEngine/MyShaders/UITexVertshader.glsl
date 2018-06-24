#version 450 core   

layout (location = 2) uniform vec2 TOPr;
layout (location = 3) uniform vec2 BOTr;

in vec2 texCoords;
out vec2 tex_coord;

void main()
{
	mat2 RM = {TOPr,BOTr};

	tex_coord = (texCoords.xy * 0.5 + 0.5);
    gl_Position = vec4((texCoords.xy)*RM, 1.0, 1.0);
}