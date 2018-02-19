#version 450 core  

uniform sampler2D ImgTexture;

layout (location=4) uniform mat4 uModel;

struct LightProperties {
	//vec3 rotationAxis;
	//float rotationAngle;
	vec4 lightColor;
	vec3 lightPosition;
};

const int NR_LIGHTS = 1;

layout (std140, binding = 0) uniform LightBlock {
    LightProperties light[NR_LIGHTS];
};

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragColor;
in vec3 fragNormal;

out vec4 FragColor;

void main(){
	vec4 surfaceColor = texture(ImgTexture, fragTexCoord) * vec4(fragColor.r,fragColor.g,fragColor.b,1.0);
	vec4 finalColor;
	vec3 AmbientLight = vec3(.1,.0,.0); //surfaceColor.rgb *

	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(uModel)));
	vec3 normal = normalize(normalMatrix * fragNormal);
	
	//calculate the location of this fragment (pixel) in world coordinates
	vec3 fragPosition = vec3(uModel * vec4(fragVert,1));

	for(int i = 0; i < NR_LIGHTS; i++){
		//calculate the vector from this pixels surface to the light source
		vec3 surfaceToLight = light[i].lightPosition - fragPosition;

		//calculate the cosine of the angle of incidence
		float brightness = dot(normal,surfaceToLight)/(length(normal)*length(surfaceToLight));
		brightness = clamp(brightness,0,1);
		
		//calculate final color of the pixel, based on:
		// 1. The angle of incidence: brightness
		// 2. The color/intensities of the light: light.intensities
		// 3. The texture and texture coord: texture(tex, fragTexCoord)
		finalColor += vec4(AmbientLight,1) + vec4(((surfaceColor.rgb) * light[i].lightColor.rgb) * (brightness * light[i].lightColor.a), surfaceColor.a);
	}
	FragColor = finalColor;
}