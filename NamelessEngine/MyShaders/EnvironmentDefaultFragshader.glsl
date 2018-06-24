#version 450 core  

#define NL_PI 3.14159265359

uniform samplerCube EnvironmentMap;

in vec3 TexCoords;
out vec4 frag_color;

void main(){
	// the sample direction equals the hemisphere's orientation (Normal)
    vec3 normal = normalize(TexCoords);

	vec3 irradiance = vec3(0.0);  
	
	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up         = cross(normal, right);
	
	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for(float phi = 0.0; phi < 2.0 * NL_PI; phi += sampleDelta)
	{
	    for(float theta = 0.0; theta < 0.5 * NL_PI; theta += sampleDelta)
	    {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 
	
	        irradiance += texture(EnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	irradiance = NL_PI * irradiance * (1.0 / float(nrSamples));
	
	//irradiance = vec3(.2f);

	frag_color = vec4(irradiance, 1.0);
	
}