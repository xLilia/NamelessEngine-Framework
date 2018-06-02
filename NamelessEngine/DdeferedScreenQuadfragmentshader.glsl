#version 450 core

//======================= DATA =========================

#define NL_PI 3.14159265359

layout (location = 0) uniform sampler2D IN_TangentFragPosColor;
layout (location = 1) uniform sampler2D IN_TangentEyePosColor_Alpha;
layout (location = 2) uniform sampler2D IN_TMatR;
layout (location = 3) uniform sampler2D IN_BMatM;
layout (location = 4) uniform sampler2D IN_NMatAo;
layout (location = 5) uniform sampler2D IN_NormalsColor_ALr;
layout (location = 6) uniform sampler2D IN_DiffuseColor_ALg;
layout (location = 7) uniform sampler2D IN_SpecularColor_ALb;
layout (location = 8) uniform sampler2D IN_DepthComponent;
layout (location = 9) uniform usampler2D IN_StencilIndex;

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
// TangentEyePosColor - view direction vector (geometry obstruction) 
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

float GeometrySmith(vec3 N, vec3 TangentEyePosColor, vec3 L, float k, int mode = 0){
	float NdotV = max(dot(N, TangentEyePosColor), 0.0);
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
//float cosTheta = max(dot(N,TangentEyePosColor), 0.0);
//float sinTheta = length(cross(N,TangentEyePosColor))/(length(N)*length(N));
//vec3 F0 = mix(F0_IOR,C,M);

vec3 FresnelSchlick(float cosTheta, vec3 F0){
	//return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

	//return mix( F0, vec3(1.0) ,  vec3(pow(1.0 - cosTheta, 5.0)));
	
	//Spherical Gaussian approximation
	float p = (-5.55473 * cosTheta -6.98316)*cosTheta;
	return F0 + (1.0 - F0) * pow(2,p);
}


void main(){

	vec4 TangentFragPosColor = texture(IN_TangentFragPosColor		, tex_coord).rgba;
	float Gmask = TangentFragPosColor.a;
		if(Gmask == 0) discard;
	vec4 TangentEyePosColor  = texture(IN_TangentEyePosColor_Alpha	, tex_coord).rgba;
	vec4 TMatR				 = texture(IN_TMatR						, tex_coord).rgba;
	vec4 BMatM				 = texture(IN_BMatM						, tex_coord).rgba;
	vec4 NMatAo				 = texture(IN_NMatAo					, tex_coord).rgba;
	vec4 NormalsColor		 = texture(IN_NormalsColor_ALr			, tex_coord).rgba;	
	vec4 DiffuseColor		 = texture(IN_DiffuseColor_ALg			, tex_coord).rgba;
	vec4 SpecularColor		 = texture(IN_SpecularColor_ALb			, tex_coord).rgba;
	vec4 DepthColor			 = texture(IN_DepthComponent			, tex_coord).rgba;
	uvec4 StencilColor		 = texture(IN_StencilIndex				, tex_coord).rgba;	//BROKEN
	
	mat3 TBNmatrix = mat3(TMatR.xyz,BMatM.xyz,NMatAo.xyz);
	float RoughnessMap = TMatR.a;
	float MetalnessMap = BMatM.a;
	float AoMap = NMatAo.a;
	float alpha = TangentEyePosColor.a;
	vec3 AlbedoMap = vec3(NormalsColor.a,DiffuseColor.a,SpecularColor.a);
	vec3 N = NormalsColor.rgb;
	vec3 V = TangentEyePosColor.rgb;

	///LIGHT CALCULATIONS

	//F0 Index of Refraction
	vec3 F0_IOR  = vec3(0.04);
	F0_IOR		 = mix(F0_IOR, AlbedoMap, MetalnessMap);

	//Reflectance Equation
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < light.length(); i++){

		vec3 TangentLightPos = TBNmatrix * light[i].lightPosition;
		vec3 TangentLightDir = TBNmatrix * light[i].lightDirection;

		//Light Radiance
		float distance;
		float attenuation;
		vec3 radiance;

		//Light direction Vector
		vec3 L;

		//Decide Light Type
		if(light[i].lightSpotInnerAngle != 0 && length(light[i].lightDirection) > 0.0){
			//SpotLight
			L = normalize(TangentLightPos - TangentFragPosColor.rgb); 
		
			float theta = dot(L, normalize(-TangentLightDir));
			float epsilon = light[i].lightSpotInnerAngle - light[i].lightSpotOuterAngle;
			float intensity = clamp((theta - light[i].lightSpotOuterAngle) / epsilon, 0.0, 1.0); 

			distance    = length(TangentLightPos - TangentFragPosColor.rgb);
			attenuation = 1.0 / (distance * distance);
			radiance     = light[i].lightColor * attenuation * intensity;

		}else if(length(light[i].lightDirection) > 0.0 && light[i].lightSpotInnerAngle == 0.0){
			//Directional Light
			radiance = light[i].lightColor/10;
			L = normalize(-TangentLightDir); 
		}else{
			//Point Light
			distance    = length(TangentLightPos - TangentFragPosColor.rgb);
			attenuation = 1.0 / (distance * distance);
			radiance     = light[i].lightColor * attenuation;
				
			L = normalize(TangentLightPos - TangentFragPosColor.rgb); 
		}

		//Halfway direction Vector
		vec3 H = normalize(V + L); 

		// Cook-Torrance BRDF //
		float NDF = NDF_GGXTR(N, H, RoughnessMap, 1); //Normal Distribution Function
		float G	  = GeometrySmith(N, V, L, RoughnessMap,1); //0 DIRECT 1 IBL //Geometry Function
		vec3 F	  = FresnelSchlick(max(dot(H, V), 0.0), F0_IOR); //Fresnel Function

		//Solving Equation
		vec3 numerator	  = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);  

		//Specular & Difuse Components
		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
		kD *= 1.0 - MetalnessMap;

		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance to Lo          
		Lo += (kD * AlbedoMap  / NL_PI + specular) * radiance * NdotL; 
	}

	vec3 ambient = (DiffuseColor.rgb + SpecularColor.rgb) * AoMap;
	
	//Final Color with ambient
	vec3 color = ambient + Lo;

	// HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

	//HDRcolor
	
	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
	
	// gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	//depthscaled
	float far = 500;
	float near = 0.1;
	vec3 eye_z = near * far / ((DepthColor.rgb * (far -near)) - far);
	vec3 DepthScaled = ( eye_z - (-near) ) / ( -far - (-near) );

	fragColor = vec4(mapped, 0);
	
}