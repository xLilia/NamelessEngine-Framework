#version 450 core

//======================= DATA =========================

#define NL_PI 3.14159265359

layout (location = 0) uniform sampler2D IN_TangentFragPosColor;
layout (location = 1) uniform sampler2D IN_TangentEyePosColor;
layout (location = 2) uniform sampler2D IN_TMatR;
layout (location = 3) uniform sampler2D IN_BMatM;
layout (location = 4) uniform sampler2D IN_NMatAo;
layout (location = 5) uniform sampler2D IN_NormalsColor;
layout (location = 6) uniform sampler2D IN_DiffuseColor;
layout (location = 7) uniform sampler2D IN_SpecularColor;

struct LightProperties {
	vec3 lightColor;
	float padding_1;
	vec3 lightPosition;
	float padding_2;
	vec3 lightDirection;
	float padding_3;
	float lightSpotInnerAngle;
	float lightSpotOuterAngle;
	float padding_4[2];
};

layout (std430, binding = 0) buffer LightBlock {
    LightProperties light[];
};

const float exposure = 2.3;
const float gamma = 0.8;

in vec2 tex_coord;
out vec4 fragColor;

//======================= FUNCTIONS =========================

///Trowbridge-Reitz GGX Normal Distribution Function
//H - Halfway Vector
//k - measure of the surface's roughness
//N - surface normal

float NDF_GGXTR(vec3 N, vec3 H, float k, int mode = 0){
	
	float a = k*k;
	float a2;
	if(mode == 0){
		a2 = k*k;
	}else{
		a2 = a*a;
	}
	
	float NdotH = max(dot(N, H),0.0);
	float NdotH2 = NdotH*NdotH;
	
	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = NL_PI * denom * denom;
	
	return nom / max(denom,0.001); // prevent divide by zero 
}

///Geometry function : GGX and Schlick-Beckmann approximation known as Schlick-GGX && Smith's method:
// k - remapping of "a" based on whether we're using the geometry function for either direct lighting or IBL lighting
// N - Normal vector
// V - view direction vector (geometry obstruction) 
// L - light direction vector (geometry shadowing)

float K_Direct(float k){
	return ((k+1)*(k+1))/8.0;
}

float K_IBL(float k){
	return (k*k)/2.0;
}

float GeometrySchlick_GGX(float NdotV, float k, int mode = 0){

	float kf = 0;
	if(mode == 0){
		kf = K_Direct(k);
	}else{
		kf = K_IBL(k);
	}

	float nom = NdotV;
	float denom = NdotV * (1.0 - kf) + kf;

	return nom / denom; 
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k, int mode = 0){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1, ggx2;
	
	ggx1 = GeometrySchlick_GGX(NdotL, k, mode);
	ggx2 = GeometrySchlick_GGX(NdotV, k , mode);
	
	return ggx1 * ggx2;
}

///Fresnel equation: Fresnel-Schlick approximation:
//N - normal
//view direction vector
//F0_IOR - Index of refraction at 0 degrees
//C - SurfaceColor
//M - Metalness
//float cosTheta = max(dot(N,V), 0.0);
//float sinTheta = length(cross(N,V))/(length(N)*length(N));
//vec3 F0 = mix(F0_IOR,C,M);

vec3 FresnelSchlick(float cosTheta, vec3 F0){
	//return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

	//return mix( F0, vec3(1.0) ,  vec3(pow(1.0 - cosTheta, 5.0)));
	
	//Spherical Gaussian approximation
	float p = (-5.55473 * cosTheta -6.98316)*cosTheta;
	return F0 + (1.0 - F0) * pow(2,p);
}


void main(){
	
	vec3 TangentFragPosColor = texture2D(IN_TangentFragPosColor , tex_coord).rgb;
	vec3 TangentEyePosColor  = texture2D(IN_TangentEyePosColor  , tex_coord).rgb;
	vec4 TMatR				 = texture2D(IN_TMatR				  , tex_coord).rgba;
	vec4 BMatM				 = texture2D(IN_BMatM				  , tex_coord).rgba;
	vec4 NMatAo				 = texture2D(IN_NMatAo				  , tex_coord).rgba;
	mat3 TBNmatrix = mat3(TMatR.xyz,BMatM.xyz,NMatAo.xyz);
	vec3 NormalsColor		 = texture2D(IN_NormalsColor		  , tex_coord).rgb;
	vec4 DiffuseColor		 = texture2D(IN_DiffuseColor		  , tex_coord).rgba;
	vec3 SpecularColor		 = texture2D(IN_SpecularColor		  , tex_coord).rgb;
	float RoughnessMap = TMatR.a;
	float MetalnessMap = BMatM.a;
	float AoMap = NMatAo.a;

	///LIGHT CALCULATIONS

	//...

	vec3 ambient = (DiffuseColor.rgb + SpecularColor) * AoMap;
	
	//Final Color with ambient
	vec3 color = ambient ;//+ Lo;

	// HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

	//HDRcolor
	
	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
	
	// gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(color,DiffuseColor.a);
	
}