#version 330 core

layout (location = 0) in vec4 aVertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
	TexCoords = aVertex.zw;
	gl_Position = projection * vec4(aVertex.xy, 0.0f, 1.0f);
}