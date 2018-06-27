#version 450 core  

layout (location = 0) uniform sampler2D image;
layout (location = 1) uniform sampler2D Previmage;
layout (location = 2) uniform vec2 mv;

in vec2 tex_coord;
in vec2 uv;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 prevfragColor;

void main(){

	vec4 c = texture2D(image, tex_coord);
	vec4 pc = texture2D(Previmage, tex_coord);
	vec4 fc = (c + pc*0.5)-c*0.5;

	prevfragColor = fc;
	fragColor = pc;
}	

	//vec4 pc;
	//int sn = 0;
	//
	//for(float s = -0.1; s < 0.1; s+=0.1){
	//for(float t = -0.1; t < 0.1; t+=0.1){
	//	sn++;
	//	vec2 modtex_coord = tex_coord + vec2(s,t);
	//	pc += texture2D(Previmage,modtex_coord);
	//}};
	//
	//pc/=sn;
	//vec4 fc = (c + pc*0.5)-c*0.5;