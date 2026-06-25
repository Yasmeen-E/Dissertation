#version 330 core

layout(location = 0) in vec3 iPosition;

uniform mat4 projectedLightSpaceMatrix;
uniform mat4 Model;

void main()
{
	
	gl_Position = projectedLightSpaceMatrix * Model * vec4(iPosition,1);
	
}
