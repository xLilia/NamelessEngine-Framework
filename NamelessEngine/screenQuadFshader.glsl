#version 450 core  
precision mediump float;

layout (location = 0) uniform sampler2D fbo_texture;

in vec2 tex_coord;
out vec4 fragColor;

float LinearizeDepth(vec2 uv)
{
    float zNear = 0.1;    
    float zFar  = 10.0;
    float depth = texture2D(fbo_texture, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main(){
	//fragColor = vec4(tex_coord,0.0,1.0);
	fragColor = texture2D(fbo_texture, tex_coord);
	//float c = LinearizeDepth(tex_coord);
	//fragColor = vec4(c,c,c,1.0);
}