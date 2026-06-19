#version 330 core


in vec2 UV;
in vec3 vNormal;

uniform sampler2D TextureSampler;

out vec3 color;
void main(){

  	color = texture( TextureSampler, UV ).rgb;

}
