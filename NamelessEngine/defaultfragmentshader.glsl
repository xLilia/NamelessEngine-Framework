#version 450 core  

in vec2 texCoord;
in vec3 vColor;

out vec4 FragColor;

uniform sampler2D ImgTexture;

void main(){
	//FragColor = vec4(1.0,0.0,1.0,1.0); 
	//FragColor = texture2D(ImgTexture,texCoord); 
	FragColor = texture2D(ImgTexture,texCoord) * vec4(vColor.r,vColor.g,vColor.b,1.0);
}