#version 450 core   

//======================= DATA =========================
const int NR_LIGHTS = 6;

layout (location=0) in vec3 aPosition;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec3 aTangent;
layout (location=3) in vec2 aTexCoords;

layout (location=4) uniform vec3 uEyePos;
layout (location=5) uniform mat4 uModel;
layout (location=6) uniform mat4 uView;
layout (location=7) uniform mat4 uProjection;

layout (location=8) uniform sampler2D AlbedoTexture;
layout (location=9) uniform sampler2D RoughnessTexture;
layout (location=10) uniform sampler2D MetalnessTexture;
layout (location=11) uniform sampler2D NormalTexture;
layout (location=12) uniform sampler2D AmbientOculusionTexture;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 vTangentLightPos[NR_LIGHTS];
out vec3 vTangentEyePos;
out vec3 vEyePos;
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

	//TBN MATRIX
	vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
	vec3 B;
	vec3 N = normalize(vec3(uModel * vec4(aNorm,	0.0)));
	
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	
	// then retrieve perpendicular vector B with the cross product of T and N
	B = cross(N, T);
	
	//transpose of an orthogonal matrix is equal to its inverse. && less expensive
	mat3 TBN = transpose(mat3(T, B, N)); 

	//Cnvert To Tangent Space

		//Lights Position in Tangent Space
		for(int i = 0; i < NR_LIGHTS; i++)
		{
			vTangentLightPos[i] = TBN * light[i].lightPosition;
		}

		//Eye Position in Tangent Space
		vTangentEyePos	   = TBN * uEyePos;

		//Fragment Position in Tangent Space
		vTangentFragPos	   = TBN * vec3(uModel * vec4(aPosition, 0.0));  

	//Fragment Tex Coordinates
	fragTexCoord = aTexCoords;

	//Fragment Position in World Space
	fragPos = vec3(uModel * vec4(aPosition,1.0));

	//Fragment Position in Screen Space
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}

//======================= END =========================