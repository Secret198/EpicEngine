#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec2 textureCoord;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

uniform mat4 model;
layout(std140) uniform ViewProjMatrix
{
    mat4 view;
    mat4 projection;
};

void main()
{
    gl_Position = projection * view * model * vec4(vertPos, 1.0);
    normal = mat3(transpose(inverse(model))) * normalVec;
    fragPos = vec3(model * vec4(vertPos, 1.0));
    texCoord = textureCoord;
    gl_PointSize = 22.5;
}