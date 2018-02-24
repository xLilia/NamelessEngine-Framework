#version 450 core  

in vec3 TexCoords;
uniform samplerCube cube_texture;
out vec4 frag_color;

void main(){
	frag_color = texture(cube_texture, TexCoords);
}