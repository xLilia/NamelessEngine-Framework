#version 450 core   

layout (location=0) in vec3 aPosition;

layout (location=1) uniform mat4 uProjection;
layout (location=2) uniform mat4 uView;
layout (location=3) uniform float uMipRoughness;

out vec3 TexCoords;

void main()
{
	TexCoords = aPosition;
	gl_Position = uProjection * uView * vec4(aPosition,1.0);
}