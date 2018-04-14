#version 450 core  

layout (location = 0) uniform sampler2D HDR_Texture;
layout (location = 1) uniform float exposure = 1.0;
layout (location = 2) uniform float gamma = 2.2;

in vec2 tex_coord;
out vec4 fragColor;

void main(){

	//get fbo Color buffer
	vec3 HDRcolor = texture2D(HDR_Texture, tex_coord).rgb;

	// reinhard tone mapping
	//vec3 mapped = HDRcolor / (HDRcolor + vec3(1.0));

	// Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-HDRcolor * exposure);
	
	// gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped,1.0);
	
}

//float LinearizeDepth(vec2 uv)
//{
//    float zNear = 0.1;    
//    float zFar  = 10.0;
//    float depth = texture2D(HDR_Texture, uv).x;
//    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
//}

//fragColor = vec4(tex_coord,0.0,1.0);
//float c = LinearizeDepth(tex_coord);
//fragColor = vec4(c,c,c,1.0);