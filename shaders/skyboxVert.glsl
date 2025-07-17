#version 330 core
layout(location = 0) in vec3 vertPos;

out vec3 TexCoords;

layout(std140) uniform ViewProjMatrix
{
    mat4 view;
    mat4 projection;
};

void main()
{
    TexCoords = vertPos;
    vec4 projectedPos = projection * mat4(mat3(view)) * vec4(vertPos, 1.0);
    gl_Position = projectedPos.xyww;
}