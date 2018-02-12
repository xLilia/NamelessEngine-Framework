#version 450 core   
precision mediump float;

const vec2 madd = vec2(0.5,0.5);

in vec2 texCoords;
out vec2 tex_coord;

void main()
{
	tex_coord = texCoords.xy * madd + madd;
    gl_Position = vec4(texCoords.xy, 0.0, 1.0);
}