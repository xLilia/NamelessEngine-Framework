#version 450 core  

uniform sampler2D equirectangularMap;

in vec3 TexCoords;
out vec4 frag_color;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv.y *= -1;
	uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main(){
	vec2 uv = SampleSphericalMap(normalize(TexCoords)); 
	vec3 color = texture(equirectangularMap, uv).rgb;

	frag_color = vec4(color, 1.0);
}