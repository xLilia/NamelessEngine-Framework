#version 450 core  

#define M_PI 3.1415926535897932384626433832795

layout (location=5) uniform mat4 uModel;

struct LightProperties {
	vec4 lightColor;
	vec3 lightPosition;
};

const int NR_LIGHTS = 99;

layout (std140, binding = 0) uniform LightBlock {
    LightProperties light[NR_LIGHTS];
};

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragColor;
in vec3 fragNormal;
layout (location=4) uniform vec3 uEyePos;

layout (location=8) uniform sampler2D AlbedoTexture;
layout (location=9) uniform sampler2D RoughnessTexture;
layout (location=10) uniform sampler2D MetalnessTexture;
layout (location=11) uniform sampler2D NormalTexture;
layout (location=12) uniform sampler2D AmbientOculusionTexture;

out vec4 FragColor;

///Trowbridge-Reitz GGX Normal Distribution Function
//H - Halfway Vector
//a - measure of the surface's roughness
//N - surface normal

float NDF_GGXTR(vec3 N, vec3 H, float a){
	
	float a2 = a*a;
	float NdotH = max(dot(N, H),0.0);
	float NdotH2 = NdotH*NdotH;
	
	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = M_PI * denom * denom;
	
	return nom / denom;
}

///Geometry function : GGX and Schlick-Beckmann approximation known as Schlick-GGX && Smith's method:
// k - remapping of "a" based on whether we're using the geometry function for either direct lighting or IBL lighting
// N - Normal vector
// V - view direction vector (geometry obstruction) 
// L - light direction vector (geometry shadowing)

float GeometrySchlick_GGX(float NdotV, float k){
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom; 
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlick_GGX(NdotV, k);
    float ggx2 = GeometrySchlick_GGX(NdotL, k);

	return ggx1 * ggx2;
}

float K_Direct(float a){
	return ((a+1)*(a+1))/8.0;
}

float K_IBL(float a){
	return (a*a)/2.0;
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
vec3 FresnelSchlick(float cosTheta, vec3 F0_IOR){
	return F0_IOR + (1.0 - F0_IOR) * pow(1.0 - cosTheta, 5.0);
}

void main(){

	//Texture Maps
	vec3 albedoMap = texture2D(AlbedoTexture, fragTexCoord).rgb;
	float roughnessMap = texture2D(RoughnessTexture, fragTexCoord).x;
	float metalnessMap = texture2D(MetalnessTexture, fragTexCoord).x;
	vec3 normalMap = texture2D(NormalTexture, fragTexCoord).rgb;
	float aoMap = texture2D(AmbientOculusionTexture, fragTexCoord).x;

	//WorldMatrix
	mat3 W_MAT = transpose(inverse(mat3(uModel)));
	
	//surface Normal vector in world Coords
	vec3 N = normalize(W_MAT * fragNormal); //vec4( 0.5*(normal + vec3(1.0, 1.0, 1.0)), 1 );
	N*= normalMap;

	//View direction vector
	vec3 V = normalize(uEyePos - fragPos);

	//Light Calculations!

	//F0 Index of Refraction
	vec3 F0_IOR  = vec3(0.04);
	F0_IOR		 = mix(F0_IOR, albedoMap, metalnessMap);

	//Reflectance Equation
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < 2; i++){
		
		//Light direction Vector
		vec3 L = normalize(light[i].lightPosition - fragPos); 

		//Halfway direction Vector
		vec3 H = normalize(L + N); 

		//Light Radiance
		float distance    = length(light[i].lightPosition - fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance     = light[i].lightColor.rgb * attenuation; 

		// Cook-Torrance BRDF //

			//Normal Distribution Function
			float NDF = NDF_GGXTR(N, H, roughnessMap);

			//Geometry Function
			float G	  = GeometrySmith(N, V, L, roughnessMap);

			//Fresnel Function
			vec3 F	  = FresnelSchlick(max(dot(H, V), 0.0), F0_IOR);

			//Specular & Difuse Components
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metalnessMap;
	
			//Solving Equation
			float NdotL		  = max(dot(N, L), 0.0);
			vec3 numerator	  = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL;
			vec3 specular     = numerator / max(denominator, 0.001);  

		// add to outgoing radiance to Lo          
        Lo += (kD * albedoMap / M_PI + specular) * radiance * NdotL; 
	}
	
	//Improvised Ambient Light
	vec3 ambient = vec3(0.03) * albedoMap * 1; //* aoMap;
   
	//Final Color with ambient
	vec3 color = ambient + Lo;
	
	//High Dynamic Range (HDR) Gama Correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));  
   
	//OUT FRAG!
    FragColor = vec4(color, 1.0);
	
}
