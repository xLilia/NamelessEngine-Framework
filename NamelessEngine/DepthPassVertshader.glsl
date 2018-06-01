#version 450 core   

//======================= DATA =========================
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in mat4 aInstanceModel;

layout(location = 8) uniform vec3 uEyePos;
layout(location = 9) uniform mat4 uView;
layout(location = 10) uniform mat4 uProjection;

out vec3 fragPos;

void main()
{
	//Fragment Position in Screen Space
	gl_Position = uProjection * uView * aInstanceModel * vec4(aPosition, 1.0);

}