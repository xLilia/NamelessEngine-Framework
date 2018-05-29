#version 450 core  

//======================= DATA =========================

#define NL_PI 3.14159265359
const int NR_LIGHTS = 1;
const float MAX_REFLECTION_LOD = 4.0;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 vTangentLightPos[NR_LIGHTS];
in vec3 vTangentLightDir[NR_LIGHTS];
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
layout (location=19) uniform int NumberOfLights;

struct LightProperties {
	vec3 lightColor;
	vec3 lightPosition;
	vec3 lightDirection;
	float lightSpotAngle;
};

layout (std430, binding = 0) buffer LightBlock {
    LightProperties light[];
};

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//======================= FUNCTIONS =========================

//vec3 getNormalFromMap()
//{
//    vec3 tangentNormal = texture(NormalTexture, fragTexCoord).xyz * 2.0 - 1.0;
//
//    vec3 Q1  = dFdx(fragPos);
//    vec3 Q2  = dFdy(fragPos);
//    vec2 st1 = dFdx(TexCoords);
//    vec2 st2 = dFdy(TexCoords);
//
//    vec3 N   = normalize(Normal);
//    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
//    vec3 B  = -normalize(cross(N, T));
//    mat3 TBN = mat3(T, B, N);
//
//    return normalize(TBN * tangentNormal);
//}

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

	vec3 albedoMap = pow( texture2D(AlbedoTexture, fragTexCoord).rgb, vec3(2.2));
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

	//Light Calculations!

	//F0 Index of Refraction
	vec3 F0_IOR  = vec3(0.04);
	F0_IOR		 = mix(F0_IOR, albedoMap, metalnessMap);

	//Reflectance Equation
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < light.length(); i++){

		//Light Radiance
		float distance;
		float attenuation;
		vec3 radiance;

		//Light direction Vector
		vec3 L;
		

		//Decide Light Type
		if(light[i].lightSpotAngle != 0 && length(light[i].lightDirection) > 0.0){
			//SpotLight
			L = normalize(vTangentLightPos[i] - vTangentFragPos); 

			float theta = dot(L, normalize(-vTangentLightDir[i]));
			theta = acos(theta);

			if(theta < light[i].lightSpotAngle){
				//distance    = length(vTangentLightPos[i] - vTangentFragPos);
				//attenuation = 1.0 / (distance * distance);
				//radiance    = light[i].lightColor * attenuation;
				radiance = vec3(0,10,0);
			}else{
				radiance = vec3(10,0,0);
			}

		}else if(length(light[i].lightDirection) > 0.0 && light[i].lightSpotAngle == 0.0){
			//Directional Light
			radiance = light[i].lightColor;
			L = normalize(-vTangentLightDir[i]); 
		}else{
			//Point Light
			distance    = length(vTangentLightPos[i] - vTangentFragPos);
			attenuation = 1.0 / (distance * distance);
			radiance     = light[i].lightColor * attenuation;
				
			L = normalize(vTangentLightPos[i] - vTangentFragPos); 
		}

		//Halfway direction Vector
		vec3 H = normalize(V + L); 

		// Cook-Torrance BRDF //
		float NDF = NDF_GGXTR(N, H, roughnessMap, 1); //Normal Distribution Function
		float G	  = GeometrySmith(N, V, L, roughnessMap,1); //0 DIRECT 1 IBL //Geometry Function
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
		kD *= 1.0 - metalnessMap;

		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance to Lo          
		Lo += (kD * albedoMap / NL_PI + specular) * radiance * NdotL; 
	}

	//Ambient Light IBL as the ambient term
	vec3 F = fresnelSchlickRoughness(max(dot(N, V),0.0), F0_IOR, roughnessMap);

	//Specular & Difuse Components
	vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalnessMap;

	//Irradience
	vec3 irradiance = texture(AmbientIrradianceTexture, N).rgb;
	vec3 diffuse = irradiance * albedoMap;

	//REFLECT
	//vec3 R = 2 * dot (N, V) * N - V;
	vec3 R = reflect(-V, N); 
	vec3 preFiltredColor = textureLod(PreFilterTexture, R, roughnessMap * MAX_REFLECTION_LOD).rgb;
	vec2 EnvBRDF = texture(BRDF2DLUTTexture, vec2( roughnessMap, max( dot(N, V) , 0.0))).rg;
	vec3 IBLspecular = preFiltredColor * (F * EnvBRDF.x * EnvBRDF.y);

	vec3 ambient = (kD * diffuse + IBLspecular) * aoMap;
	
	//Final Color with ambient
	vec3 color = ambient + Lo;

	// HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
   
	//OUT FRAG!
    FragColor = vec4(color,1.0);
    //FragColor = vec4(albedoMap,1.0);

	//Brightness
	float Luma = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	if(Luma > 1.0){
		BrightColor = vec4(FragColor.rgb,1.0);
	}else{
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

//======================= END =========================