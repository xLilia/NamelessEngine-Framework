#version 450 core   

layout (location=0) in vec3 aPosition;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec3 aColor;
layout (location=3) in vec2 aTexCoords;
layout (location=4) uniform mat4 MVP;

out vec2 texCoord;
out vec3 vColor;

void main()
{
	vColor = aColor;//aPosition*2;
	texCoord = aTexCoords;
    gl_Position = MVP * vec4(aPosition, 1.0);
}