#version 330 core


in vec2 UV;
in vec3 vNormal;
in vec4 FragPosProjectedLightSpace;

uniform sampler2D TextureSampler;
uniform sampler2D shadowMap;

uniform vec3 SnowDirection;


out vec3 color;

float noise(vec2 uv)
{
    return fract(sin(dot(uv, vec2(127.1, 311.7))) * 43758.5453);
}

float shadowPCFFragment(vec4 FragPosProjectedLightSpace)
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



    float shadow    = 0.0;
    vec2  texelSize = 1.0 / vec2(textureSize(shadowMap, 0).xy);

    int   kernelRadius = 3;  // 3 = 7x7,  2 = 5x5,  1 = 3x3
    float spread       = 5.0; // boundary area

    float dzdx      = dFdx(fragDepth);  
    float dzdy      = dFdy(fragDepth);

    for(int x = -kernelRadius; x <= kernelRadius; x++)
    {
        for(int y = -kernelRadius; y <= kernelRadius; y++)
        {
        vec2  off        = vec2(x, y) * texelSize * spread;
        float depthAtOff = fragDepth + dzdx * off.x + dzdy * off.y; 
        float depth      = texture(shadowMap, ss.xy + off).r;
        shadow += depthAtOff > (depth + bias) ? 1.0 : 0.0;
        }
    }

    float total = float((kernelRadius * 2 + 1) * (kernelRadius * 2 + 1));
    shadow /= total;
 return shadow;

}

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


    float shadow = shadowPCFFragment(FragPosProjectedLightSpace);

    //float shadow = shadowOnFragment(FragPosProjectedLightSpace);

    vec3 ambient = texture( TextureSampler, UV ).rgb;

    vec3 N = normalize(vNormal);
    //A = E * ( N . Up )
    float E = 1.f- shadow;

    vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	  vec3 ss = (ndc+1) * 0.5;

    if(E > 0.0)
  {

    float noise = noise(ss.xy) * 0.5;
    E  = clamp(E + noise, 0.0, 1.0);

  }
    float A   = E * max(dot(N, vec3(0.0, 1.0, 0.0)), 0.0);


    //color = ambient +  (1.f - shadow);
    vec3 snowColour = vec3(1.0, 1.0, 1.0); 
    color = mix(ambient, snowColour, A);


  //	color = texture( TextureSampler, UV ).rgb;

}
