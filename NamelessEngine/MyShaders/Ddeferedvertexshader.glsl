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
out vec2 fragTexCoord;
out mat3 TBN;

//======================= VERTEX SHADER =========================

void main()
{

	//Fragment Tex Coordinates
	fragTexCoord = aTexCoords;

	//Fragment Position in World Space
	fragPos = vec3(aInstanceModel * vec4(aPosition, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(aInstanceModel)));

	//TBN MATRIX
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNorm);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = transpose(mat3(T, B, N));

	//Fragment Position in Screen Space
	gl_Position = uProjection * uView * aInstanceModel * vec4(aPosition, 1.0);
}

//======================= END =========================