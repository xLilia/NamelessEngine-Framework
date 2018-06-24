#version 450 core   

in vec2 texCoords;
out vec2 tex_coord;
out vec2 uv;

void main()
{
	tex_coord = texCoords.xy * 0.5 + 0.5;
	uv = texCoords.xy;
	//uv.x /= 0.567;
    gl_Position = vec4(texCoords.xy, 0.0, 1.0);
}