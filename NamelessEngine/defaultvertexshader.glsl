#version 450 core   

layout (location=0) in vec3 aPosition;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec3 aColor;
layout (location=3) in vec2 aTexCoords;

layout (location=4) uniform mat4 uModel;
layout (location=5) uniform mat4 uView;
layout (location=6) uniform mat4 uProjection;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragColor;
out vec3 fragNormal;

void main()
{
	fragColor = aColor;
	fragNormal = aNorm;
	fragTexCoord = aTexCoords;
	fragPos = vec3(uModel * vec4(aPosition,1.0));
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    //gl_Position = MVP * vec4(aPosition, 1.0);
}