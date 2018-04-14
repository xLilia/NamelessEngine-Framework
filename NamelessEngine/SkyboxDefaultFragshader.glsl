#version 450 core  

uniform samplerCube cube_texture;
layout (location=5) uniform float mipLvl;

in vec3 TexCoords;
out vec4 frag_color;

void main(){
	frag_color = textureLod(cube_texture, TexCoords, mipLvl);
}
