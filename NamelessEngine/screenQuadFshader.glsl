#version 450 core  

layout (location = 0) uniform sampler2D HDR_Texture;
layout (location = 1) uniform sampler2D bloomBlur;

const float exposure = 2.0;
const float gamma = 1.0;

in vec2 tex_coord;
out vec4 fragColor;

void main(){

	//get fbo Color buffer
	vec3 HDRcolor = texture2D(HDR_Texture, tex_coord).rgb;
	vec3 bloomColor = texture2D(bloomBlur, tex_coord).rgb;

	HDRcolor += bloomColor; //additive blending

	// reinhard tone mapping
	//vec3 mapped = HDRcolor / (HDRcolor + vec3(1.0));

	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-HDRcolor * exposure);
	
	// gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped,1.0);
	
}