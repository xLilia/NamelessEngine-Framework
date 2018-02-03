#version 330 core

uniform sampler2D state;
uniform vec2 scale0;

in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

vec4 get(vec2 offset){
	return texture(state, (gl_FragCoord.xy + offset) / scale0); 
}

vec4 sum(int d){
	vec4 L =		get(vec2(-d,0));
	vec4 R =		get(vec2(+d,0));
	vec4 U =		get(vec2(0,+d));
	vec4 D =		get(vec2(0,-d));
	vec4 UL =		get(vec2(-d,+d));
	vec4 UR =		get(vec2(+d,+d));
	vec4 DL =		get(vec2(-d,-d));
	vec4 DR =		get(vec2(+d,-d));
	return			L+R+U+D+UL+UR+DL+DR;
}

void main(){
	////Init
	vec4 current =	get(vec2(0,0));
	vec4 L =		get(vec2(-1,0));
	vec4 R =		get(vec2(+1,0));
	vec4 U =		get(vec2(0,+1));
	vec4 D =		get(vec2(0,-1));
	vec4 UL =		get(vec2(-1,+1));
	vec4 UR =		get(vec2(+1,+1));
	vec4 DL =		get(vec2(-1,-1));
	vec4 DR =		get(vec2(+1,-1));
	vec4 birth =	vec4(1);
	vec4 death =	vec4(0);
	
	////Rules
	//outColor.r = ((current+sum(1))/9).r;
	//outColor.g = ((current+sum(1))/9).g;

	if(float(sum(1)) > 2){
		outColor -= sum(1);
	}


	


	
}