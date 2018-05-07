#version 450 core  

#define NL_PI 3.14159265359
const int NR_LIGHTS = 1;
const float MAX_REFLECTION_LOD = 4.0;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 vTangentLightPos[NR_LIGHTS];
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

struct LightProperties {
	vec3 lightColor;
	vec3 lightPosition;
	vec3 lightDirection;
	float lightSpotAngle;
};

layout (std140, binding = 0) uniform LightBlock {
    LightProperties light[NR_LIGHTS];
};

layout (location = 0) out vec4 FragColor;

void main(){
	vec4 color = texture(AlbedoTexture, fragTexCoord);
	FragColor = vec4(color.rgb*7,color.a);
}