#version 330

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    // FragColor = texture(screenTexture, textureCoords);
    FragColor = vec4(vec3(1.0 - texture(screenTexture, textureCoords)), 1.0);
}