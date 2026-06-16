#version 330 core
layout(location = 0) in vec3 iPosition;



layout(std140) uniform UScene {
    mat4 view;
    mat4 projection;
    mat4 projView;
    vec4 camPos;
} uScene;

void main(){
	gl_Position =  uScene.projView * vec4(iPosition,1);
}
