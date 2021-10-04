#version 330
layout(location = 0) in vec3 position;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Proj * u_View * vec4(position, 1.0f);
}