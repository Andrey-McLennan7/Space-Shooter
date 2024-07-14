#version 330 core

out vec4 FragColour;

uniform vec3 uColour;

void main()
{
	FragColour = vec4(uColour, 1.0f);
}