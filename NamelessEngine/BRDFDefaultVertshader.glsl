#version 450 core   

in vec2 texCoords;
out vec2 TexCoords;

void main()
{
	TexCoords = texCoords.xy * 0.5 + 0.5;
    gl_Position = vec4(texCoords.xy, 0.0, 1.0);
}