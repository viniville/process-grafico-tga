#version 430 core
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
	color = texture(ourTexture1, TexCoord);
}