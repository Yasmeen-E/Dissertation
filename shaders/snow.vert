#version 330 core
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexture;
layout(location = 2) in vec3 iNormal;


layout(std140) uniform UScene {
    mat4 view;
    mat4 projection;
    mat4 projView;
    vec4 camPos;
} uScene;

uniform mat4 projectedLightSpaceMatrix;
uniform mat4 Model;
uniform float t;

uniform sampler2D shadowMap;
uniform sampler3D NoiseSampler;

out vec3 vNormal;
out vec4 FragPosProjectedLightSpace;
out vec3 FragPosWorldSpace;
out vec3 camPos;


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


//P ' = P x t x F x A

void main(){


 vNormal = normalize(mat3(transpose(inverse(Model))) * iNormal);

float incline = max(dot(vNormal, vec3(0.0, 1.0, 0.0)), 0.0);

vec3 worldPos = (Model * vec4(iPosition, 1.0)).xyz;

vec3 displacedWorldPos = worldPos + incline * vNormal * t  ;

gl_Position = uScene.projView * vec4(displacedWorldPos, 1.0);
FragPosProjectedLightSpace = projectedLightSpaceMatrix * vec4(displacedWorldPos, 1.0);
FragPosWorldSpace = displacedWorldPos;
camPos = vec3(uScene.camPos);
    


    //vNormal = normalize(mat3(transpose(inverse(Model))) *iNormal);
    //float incline = max(dot(vNormal, vec3(0.0, 1.0, 0.0)), 0.0);
    //vec3 position = iPosition + incline * vNormal * t ;
	//gl_Position =  uScene.projView * Model *vec4(position,1);
    //FragPosProjectedLightSpace = projectedLightSpaceMatrix * Model * vec4(position,1);
    //FragPosWorldSpace = (Model * vec4(position,1)).xyz;
    //camPos = vec3(uScene.camPos); 

}
