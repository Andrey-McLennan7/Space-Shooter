#version 330 core

out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D uText;
uniform vec3 uTextColour;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uText, TexCoords).r);
	FragColour = vec4(uTextColour, 1.0f) * sampled;
}