#version 450 core

//======================= DATA =========================

#define NL_PI 3.14159265359

layout (location = 0) uniform sampler2D PositionColor;
layout (location = 1) uniform sampler2D NormalsColor;
layout (location = 2) uniform sampler2D DiffuseColor;
layout (location = 3) uniform sampler2D RoughMetalColor;

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
	
	vec3 PosMap = texture2D(PositionColor, tex_coord).rgb;
	vec3 NormMap = texture2D(NormalsColor, tex_coord).rgb;
	vec4 DiffMap = texture2D(DiffuseColor, tex_coord).rgba;
	vec2 RoughMetalMap = texture2D(RoughMetalColor, tex_coord).rg;
	float roughnessMap = RoughMetalMap.x;
	float metalnessMap = RoughMetalMap.y;


	vec3 Lo = vec3(0.0);

	for(int i = 0; i < light.length(); i++){

		if(length(light[i].lightPosition - PosMap) < 10){
			Lo = vec3(0,10.0,0);
		}else{
			Lo = vec3(10.0,0,0);
		}
		
	}


	//Final Color with ambient
	vec3 color = DiffMap.rgb ;//* Lo;
	
	// HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
	
	///HDRcolor
	
	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
	
	// gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(PosMap,DiffMap.a);
	
}