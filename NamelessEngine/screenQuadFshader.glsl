#version 450 core  
precision mediump float;

uniform sampler2D fbo_texture;

in vec2 tex_coord;
out vec4 fragColor;

void main(){
	fragColor = vec4(tex_coord,0.0,1.0);
	//fragColor = texture2D(fbo_texture, tex_coord);
}