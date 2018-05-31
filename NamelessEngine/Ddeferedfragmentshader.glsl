#version 450 core

//======================= DATA =========================

#define NL_PI 3.14159265359

const float MAX_REFLECTION_LOD = 4.0;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 vTangentEyePos;
in vec3 vTangentFragPos;

layout (location=11) uniform sampler2D AlbedoTexture;
layout (location=12) uniform sampler2D RoughnessTexture;
layout (location=13) uniform sampler2D MetalnessTexture;
layout (location=14) uniform sampler2D NormalTexture;
layout (location=15) uniform sampler2D AmbientOculusionTexture;
layout (location=16) uniform samplerCube AmbientIrradianceTexture;
layout (location=17) uniform samplerCube PreFilterTexture;
layout (location=18) uniform sampler2D BRDF2DLUTTexture;

layout (location = 0) out vec4 OUT_PositionColor;
layout (location = 1) out vec4 OUT_NormalsColor;
layout (location = 2) out vec4 OUT_DiffuseColor;
layout (location = 3) out vec4 OUT_RoughMetalColor;

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

	//Texture Maps

	vec4 albedoMap = texture2D(AlbedoTexture, fragTexCoord);
	albedoMap.rgb = pow( albedoMap.rgb, vec3(2.2));
	float roughnessMap = texture2D(RoughnessTexture, fragTexCoord).x;
	float metalnessMap = texture2D(MetalnessTexture, fragTexCoord).x;
	vec3 normalMap = texture2D(NormalTexture, fragTexCoord).rgb;
	float aoMap = texture2D(AmbientOculusionTexture, fragTexCoord).x;
	
	//Perfect Materials

	//vec3 albedoMap = pow( vec3(1,1,1), vec3(2.2) );
	//float roughnessMap = 0.0;
	//float metalnessMap = 1.0;
	//vec3 normalMap = vec3(0.5,0.5,1.0);
	//float aoMap = 1.0;

	//NORMAL
	vec3 N = normalMap;
    N = normalize(N * 2.0 - 1.0);

	//View direction vector
	vec3 V = normalize(vTangentEyePos - vTangentFragPos);

	//F0 Index of Refraction
	vec3 F0_IOR  = vec3(0.04);
	F0_IOR		 = mix(F0_IOR, albedoMap.rgb, metalnessMap);

	//Reflectance Equation
	vec3 Lo = vec3(0.0);

	//Ambient Light IBL as the ambient term
	vec3 F = fresnelSchlickRoughness(max(dot(N, V),0.0), F0_IOR, roughnessMap);

	//Specular & Difuse Components
	vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalnessMap;

	//Irradience
	vec3 irradiance = texture(AmbientIrradianceTexture, N).rgb;
	vec3 diffuse = irradiance * albedoMap.rgb;
	
	//REFLECT
	//vec3 R = 2 * dot (N, V) * N - V;
	vec3 R = reflect(-V, N); 
	vec3 preFiltredColor = textureLod(PreFilterTexture, R, roughnessMap * MAX_REFLECTION_LOD).rgb;
	vec2 EnvBRDF = texture(BRDF2DLUTTexture, vec2( roughnessMap, max( dot(N, V) , 0.0))).rg;
	vec3 IBLspecular = preFiltredColor * (F * EnvBRDF.x * EnvBRDF.y);
	
	vec3 ambient = (kD * diffuse + IBLspecular) * aoMap;

	//OUT FRAG!

	OUT_PositionColor = vec4(fragPos,1.0);
	OUT_NormalsColor = vec4(N,1.0);
	OUT_DiffuseColor = vec4(ambient,albedoMap.a);
	OUT_RoughMetalColor = vec4(roughnessMap,metalnessMap,1.0,1.0);
}

//======================= END =========================