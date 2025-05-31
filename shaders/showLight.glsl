#version 330

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    fragColor = texture(texture_diffuse1, texCoord);
}