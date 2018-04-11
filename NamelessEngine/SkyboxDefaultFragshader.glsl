#version 450 core  

uniform samplerCube cube_texture;

in vec3 TexCoords;
out vec4 frag_color;

void main(){
	frag_color = texture(cube_texture, TexCoords);
}
