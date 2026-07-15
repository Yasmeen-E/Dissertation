#version 330 core


in vec2 UV;
in vec3 vNormal;
in vec4 FragPosProjectedLightSpace;
in vec3 FragPosWorldSpace;
in vec3 camPos;

uniform sampler2D shadowMap;
uniform sampler3D NoiseSampler;

uniform vec3 lightDirection;
uniform vec3 SnowDirection;


out vec4 color;

//remove later
float noise(vec2 uv) 
{
    return fract(sin(dot(uv, vec2(127.1, 311.7))) * 43758.5453);
}


vec3 sampleNoiseOctaves(vec3 p) {
    vec3 sum = vec3(0.0);
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 3; i++) {
        sum += texture(NoiseSampler, p * frequency).rgb * amplitude;
        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return sum * 2.0 - 1.0;
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

//Nα = N +αn − dE
//distores normal for snow colouring
//N = normal
//n = noise vector (normalised)
// α = scalar , different values for specular and diffuse
//dE = derivatives of exposure function

vec3 GetDistoredNormal(vec3 normal, float scalar)
{

    float noisescalar = 1.f;
    vec3 n = normalize(sampleNoiseOctaves(FragPosWorldSpace * noisescalar));

    float E =  1.f - shadowPCFFragment(FragPosProjectedLightSpace);
    float dEx = dFdx(E);
    float dEy = dFdy(E);

    vec3 dE = vec3(dEx, 0.0, dEy);

    return normal + n*scalar - dE;

}



float PhongSnowLighting()
{

    float ambient = 0.6f;

	vec3 Nnor = normalize(vNormal);
	vec3 Nto_light = normalize(-lightDirection);

    vec3 Ndiffuse = normalize(GetDistoredNormal(Nnor, 0.6));
    vec3 Nspec = normalize(GetDistoredNormal(Nnor, 0.9));

    float diffuse = max(dot(Ndiffuse,Nto_light),0);

	vec3 Nfrom_light= normalize(lightDirection);
    vec3 NrefLightSpec = reflect(Nfrom_light,Nspec);
	vec3 camDirection = camPos-FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

	float spec = pow(max(dot(NcamDirection,NrefLightSpec),0),128);

	return ambient + diffuse + spec ;

}

vec3 PhongSnowColour()
{

  return vec3(0.90, 0.90, 1.0); 

}

void main(){


   


    vec3 raw = texture(NoiseSampler, FragPosWorldSpace * 0.5).rgb;
   // color = vec4(raw, 1.0);


    float shadow = shadowPCFFragment(FragPosProjectedLightSpace);

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

    float A = E * max(dot(N, vec3(0.0, 1.0, 0.0)), 0.0);
    vec3 snowColor = PhongSnowColour() * PhongSnowLighting(); 



    if (A < 0.01)
        discard;
     
    //color = vec4(raw, 1.0);
    color =  vec4(snowColor, A);

}
