#version 450 core  

layout (location = 0) uniform sampler2D Texture;

in vec2 tex_coord;
out vec4 fragColor;

void main(){
	
	vec4 color = texture2D(Texture, tex_coord).rgba;
	fragColor = vec4(color);
	
}