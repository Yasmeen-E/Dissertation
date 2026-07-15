#version 330 core


in vec2 UV;
in vec3 vNormal;
in vec3 FragPosWorldSpace;

uniform sampler2D TextureSampler;
uniform vec3 lightDirection;
out vec3 color;
in vec3 camPos;

vec3 PhongDirectionalLighting()
{

    float ambient = 0.5f;

	vec3 Nnor = normalize(vNormal);
	vec3 Nto_light = normalize(-lightDirection);
	float diffuse = max(dot(Nnor,Nto_light),0);

	vec3 Nfrom_light= normalize(lightDirection);
	vec3 camDirection = camPos-FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

    vec3 texture = texture( TextureSampler, UV ).rgb;
	return (ambient + diffuse) * texture;

}


void main(){

      color = PhongDirectionalLighting();

}
