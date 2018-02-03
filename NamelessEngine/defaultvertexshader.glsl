#version 330 core

in vec2 position;
in vec2 texCoords;
in vec3 colors;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	Color = colors;
	TexCoord = texCoords;
    gl_Position = vec4(position, 0.0, 1.0);
}