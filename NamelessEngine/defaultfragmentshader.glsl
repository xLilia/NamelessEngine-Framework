#version 450 core  

layout (location=4) uniform mat4 uModel;

struct LightProperties {
	vec4 lightColor;
	vec3 lightPosition;
	float radiusOfInfluence;
};

const int NR_LIGHTS = 99;

layout (std140, binding = 0) uniform LightBlock {
    LightProperties light[NR_LIGHTS];
};

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragColor;
in vec3 fragNormal;

layout (location=7) uniform sampler2D AlbedoTexture;
//layout (location=8) uniform sampler2D RoughnessTexture;
//layout (location=9) uniform sampler2D MetalnessTexture;
//layout (location=10) uniform sampler2D NormalTexture;
//layout (location=11) uniform sampler2D AmbientOculusionTexture;

out vec4 FragColor;

void main(){
	vec4 finalColor = texture2D(AlbedoTexture, fragTexCoord);
	FragColor = finalColor;
}
