#version 450 core

//======================= DATA =========================

#define NL_PI 3.14159265359

const float MAX_REFLECTION_LOD = 4.0;

in vec3 fragPos;
in vec2 fragTexCoord;
in mat3 TBN;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in mat4 aInstanceModel;

layout(location = 8) uniform vec3 uEyePos;
layout(location = 9) uniform mat4 uView;
layout(location = 10) uniform mat4 uProjection;

layout (location=11) uniform sampler2D AlbedoTexture;
layout (location=12) uniform sampler2D RoughnessTexture;
layout (location=13) uniform sampler2D MetalnessTexture;
layout (location=14) uniform sampler2D NormalTexture;
layout (location=15) uniform sampler2D AmbientOculusionTexture;
layout (location=16) uniform samplerCube AmbientIrradianceTexture;
layout (location=17) uniform samplerCube PreFilterTexture;
layout (location=18) uniform sampler2D BRDF2DLUTTexture;

layout (location = 0) out vec4 OUT_TangentFragPosColor;
layout (location = 1) out vec4 OUT_TangentEyePosColor_Alpha;
layout (location = 2) out vec4 OUT_TMatR;
layout (location = 3) out vec4 OUT_BMatM;
layout (location = 4) out vec4 OUT_NMatAo;
layout (location = 5) out vec4 OUT_NormalsColor_ALr;
layout (location = 6) out vec4 OUT_DiffuseColor_ALg;
layout (location = 7) out vec4 OUT_SpecularColor_ALb;

//======================= FUNCTIONS =========================

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    //return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);

	//Spherical Gaussian approximation
	float p = (-5.55473 * cosTheta -6.98316)*cosTheta;
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(2,p);
}

//======================= FRAGMENT SHADER =========================

void main(){

	//Optimize a bit
	if(fragTexCoord.x == 0) discard;
	if(fragTexCoord.y == 0) discard;

	//Texture Maps

	vec4 albedoMap = texture2D(AlbedoTexture, fragTexCoord);
	albedoMap.rgb = pow( albedoMap.rgb, vec3(2.2));
	float roughnessMap = texture2D(RoughnessTexture, fragTexCoord).x;
	float metalnessMap = texture2D(MetalnessTexture, fragTexCoord).x;
	vec3 normalMap = texture2D(NormalTexture, fragTexCoord).rgb;
	float aoMap = texture2D(AmbientOculusionTexture, fragTexCoord).x;
	
	//Perfect Materials (FOR DEBUGGING)

	//vec4 albedoMap = vec4(1,1,1,1);
	//albedoMap.rgb = pow( albedoMap.rgb, vec3(2.2));
	//float roughnessMap = 0.0;
	//float metalnessMap = 1.0;
	//vec3 normalMap = vec3(0.5,0.5,1.0);
	//float aoMap = 1.0;

	//NORMAL
	vec3 N = normalMap;
    N = normalize(N * 2.0 - 1.0);

	vec3 vTangentFragPos = TBN * fragPos;
	vec3 vTangentEyePos = TBN * uEyePos;

	//View direction vector
	vec3 V = normalize(vTangentEyePos - vTangentFragPos);

	//F0 Index of Refraction
	vec3 F0_IOR  = vec3(0.04);
	F0_IOR		 = mix(F0_IOR, albedoMap.rgb, metalnessMap);

	//Ambient Light IBL as the ambient term
	vec3 F = fresnelSchlickRoughness(max(dot(N, V),0.0), F0_IOR, roughnessMap);

	//Specular & Difuse Components
	vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalnessMap;

	//Irradience
	vec3 irradiance = texture(AmbientIrradianceTexture, N).rgb;
	vec3 diffuse = irradiance * albedoMap.rgb * kD;

	//REFLECT
	//vec3 R = 2 * dot (N, V) * N - V;
	vec3 R = reflect(-V, N); 
	vec3 preFiltredColor = textureLod(PreFilterTexture, R, roughnessMap * MAX_REFLECTION_LOD).rgb;
	vec2 EnvBRDF = texture(BRDF2DLUTTexture, vec2( roughnessMap, max( dot(N, V) , 0.0))).rg;
	vec3 IBLspecular = preFiltredColor * (F * EnvBRDF.x * EnvBRDF.y);

	//OUT FRAG!

	OUT_TangentFragPosColor = vec4(vTangentFragPos,fragTexCoord.x);
	OUT_TangentEyePosColor_Alpha = vec4(V,albedoMap.a);

	OUT_TMatR = vec4(TBN[0],roughnessMap);
	OUT_BMatM = vec4(TBN[1],metalnessMap);
	OUT_NMatAo = vec4(TBN[2],aoMap);
	
	OUT_NormalsColor_ALr = vec4(N,albedoMap.r);
	OUT_DiffuseColor_ALg = vec4(diffuse,albedoMap.g);
	OUT_SpecularColor_ALb = vec4(IBLspecular,albedoMap.b);
}

//======================= END =========================