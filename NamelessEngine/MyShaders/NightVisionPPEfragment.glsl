#version 450 core  

layout (location = 0) uniform sampler2D image;
layout (location = 1) uniform sampler2D previmage;

in vec2 tex_coord;
in vec2 uv;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 prevfragColor;

void main(){

	vec4 c = texture2D(image, tex_coord);
	vec4 pc = texture2D(previmage, tex_coord);
	
	float mod = 1-length(uv);
	
	float col = c.r + c.g + c.b; 
	col*=mod;
	col*=5;
	float pcol = pc.r + pc.g + pc.b; 

	float fc = (col+pcol*0.75)-col*0.75;

	prevfragColor = vec4(0,fc,0,0);
	fragColor = pc;

}

	//float pcol = pc.r + pc.g + pc.b; 

	//if(pcol <= 0) pcol = 0.5;
	//
	//float fc = (col*10 * pcol) *col;
	//
	//fc *= 10;
	//if(fc >= 1) fc = 0;
	//
	//prevfragColor = vec4(0,fc,0,0);
	//fragColor = pc*2;
	//prevfragColor = c + pc*0.9;
	//fragColor = pc;