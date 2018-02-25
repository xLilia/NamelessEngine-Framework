#version 450 core  

uniform sampler2D ImgTexture;

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

out vec4 FragColor;

void main(){
	vec3 finalColor;
	vec4 surfaceColor = texture(ImgTexture, fragTexCoord) * vec4(fragColor.r,fragColor.g,fragColor.b,1.0);
	
	vec3 AmbientLight = vec3(1,1,1); 
	vec3 Ambient = AmbientLight * .3;

	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(uModel)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	for(int i = 0; i < NR_LIGHTS; i++){
		//calculate the vector from this pixels surface to the light source
		vec3 lightDir = light[i].lightPosition - fragPos;
		
		//Calculate Distance from this pixels surface to the light source
		float fragDistanceToLight = length(lightDir);

		//calculate the cosine of the angle of incidence
		
		float diff = max(dot(normal, normalize(lightDir)), 0.0);
		vec3 diffuse = diff * light[i].lightColor.rgb;

		//float brightness = dot(normal,lightDir)/(length(normal)*length(lightDir));
		//brightness = clamp(brightness,0,1);
		
		//calculate final color of the pixel, based on:
		// 1. The angle of incidence: brightness
		// 2. The color/intensities of the light: light.intensities
		// 3. The texture and texture coord: texture(tex, fragTexCoord)
		
		//vec4 DifuseLight = vec4(((surfaceColor.rgb) * light[i].lightColor.rgb) * (brightness * light[i].lightColor.a), surfaceColor.a);

		vec3 LightSphere = 1-vec3(fragDistanceToLight) / 4 ;//* max(light[i].radiusOfInfluence,0); //vec4((1-vec3(fragDistanceToLight)/light[i].radiusOfInfluence),1);

		finalColor = (Ambient + ( diffuse * LightSphere)) ;

	}
	FragColor = vec4(finalColor,1.0) * surfaceColor;
}