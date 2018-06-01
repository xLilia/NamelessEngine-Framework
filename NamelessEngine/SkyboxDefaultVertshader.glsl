#version 450 core  

layout (location=0) in vec3 aPosition;
layout (location=1) uniform mat4 uProjection;
layout (location=2) uniform mat4 uView;

out vec3 TexCoords;

void main(){
	TexCoords = aPosition;
	vec4 FarPos = uProjection * uView * vec4(aPosition,1.0);
	FarPos.z = FarPos.w;
	gl_Position = FarPos;
}
	