#version 450 core   

//======================= DATA =========================
const int NR_LIGHTS = 2;

layout (location=0) in vec3 aPosition;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec3 aTangent;
layout (location=3) in vec2 aTexCoords;
layout (location=4) in mat4 aInstanceModel;
//location=5
//location=6
//location=7

layout (location=8) uniform vec3 uEyePos;
//layout (location=5) uniform mat4 uModel;
layout (location=9) uniform mat4 uView;
layout (location=10) uniform mat4 uProjection;

layout (location=11) uniform sampler2D AlbedoTexture;
layout (location=12) uniform sampler2D RoughnessTexture;
layout (location=13) uniform sampler2D MetalnessTexture;
layout (location=14) uniform sampler2D NormalTexture;
layout (location=15) uniform sampler2D AmbientOculusionTexture;
layout (location=16) uniform samplerCube AmbientIrradianceTexture;
layout (location=17) uniform samplerCube PreFilterTexture;
layout (location=18) uniform sampler2D BRDF2DLUTTexture;

out vec3 fragPos;
out vec2 fragTexCoord;
//out vec3 Normal;
out vec3 vTangentLightPos[NR_LIGHTS];
out vec3 vTangentEyePos;
out vec3 vTangentFragPos;

struct LightProperties {
	vec3 lightColor;
	vec3 lightPosition;
};

layout (std140, binding = 0) uniform LightBlock {
    LightProperties light[NR_LIGHTS];
};

//======================= VERTEX SHADER =========================

void main()
{
	//Normal = mat3(aInstanceModel) * aNorm;

	//Fragment Tex Coordinates
	fragTexCoord = aTexCoords;

	//Fragment Position in World Space
	fragPos = vec3(aInstanceModel * vec4(aPosition,1.0));


	mat3 normalMatrix = transpose(inverse(mat3(aInstanceModel)));

	vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNorm);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N)); 

	//TBN MATRIX
	//vec3 T = normalize(vec3(aInstanceModel * vec4(aTangent, 0.0)));
	//vec3 B;
	//vec3 N = normalize(vec3(aInstanceModel * vec4(aNorm,	0.0)));
	//
	//// re-orthogonalize T with respect to N
	//T = normalize(T - dot(T, N) * N);
	//
	//// then retrieve perpendicular vector B with the cross product of T and N
	//B = cross(N, T);
	//
	////transpose of an orthogonal matrix is equal to its inverse. && less expensive
	//mat3 TBN = transpose(mat3(T, B, N)); 

	//Cnvert To Tangent Space

		//Lights Position in Tangent Space
		for(int i = 0; i < NR_LIGHTS; i++)
		{
			vTangentLightPos[i] = TBN * light[i].lightPosition;
		}

		//Eye Position in Tangent Space
		vTangentEyePos	   = TBN * uEyePos;

		//Fragment Position in Tangent Space
		vTangentFragPos	   = TBN * fragPos;  

	//Fragment Position in Screen Space
	gl_Position = uProjection * uView * aInstanceModel * vec4(aPosition, 1.0);
}

//======================= END =========================