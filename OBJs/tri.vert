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

out vec2 UV;
out vec3 vNormal;
out vec4 FragPosProjectedLightSpace;
out vec3 FragPosWorldSpace;

void main(){
	gl_Position =  uScene.projView * Model *vec4(iPosition,1);
   	UV = iTexture;
    vNormal = mat3(transpose(inverse(Model))) *iNormal;
    FragPosProjectedLightSpace = projectedLightSpaceMatrix * Model * vec4(iPosition,1);
    FragPosWorldSpace = vec3(Model * vec4(iPosition,1));

}
