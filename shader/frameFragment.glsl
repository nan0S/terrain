#version 330 core

flat in vec3 vColor;
out vec3 color;

void main()
{
	color = vColor;
}