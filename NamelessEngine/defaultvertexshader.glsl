#version 450 core   

layout (location=0) in vec3 aPosition;
layout (location=3) in vec2 aTexCoords;
layout (location=4) uniform mat4 MVP;

out vec2 texCoord;

void main()
{
	texCoord = aTexCoords;
    gl_Position = MVP * vec4(aPosition, 1.0);
}