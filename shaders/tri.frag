#version 330 core


in vec2 UV;
in vec3 vNormal;
in vec4 FragPosProjectedLightSpace;

uniform sampler2D TextureSampler;
uniform sampler2D shadowMap;

uniform vec3 SnowDirection;


out vec3 color;

float shadowOnFragment(vec4 FragPosProjectedLightSpace)
{
	vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	vec3 ss = (ndc+1) * 0.5;

  if(ss.x < 0.0 || ss.x > 1.0 || ss.y < 0.0 || ss.y > 1.0)
        return 0.0;

	float fragDepth = ss.z;
	float litDepth = texture(shadowMap, ss.xy).r;

	vec3 Nnor = normalize(vNormal);
	vec3 Ntolight = normalize(-SnowDirection);
	float bias = max(0.005 * (1.0 - dot(Nnor, Ntolight)),0.001);

	float shadow = 0.f;
	shadow = fragDepth > (litDepth + bias) ? 1.0:0.0;

	if(fragDepth > 1)
		shadow = 0.f;

	return shadow;


}

void main(){


    float shadow = shadowOnFragment(FragPosProjectedLightSpace);
    vec3 ambient = texture( TextureSampler, UV ).rgb;



    color = ambient * (1.f - shadow);


  //	color = texture( TextureSampler, UV ).rgb;

}
