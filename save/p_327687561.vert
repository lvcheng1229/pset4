#version 450

layout(location = 0) in vec3 atParam0;
layout(location = 1) in vec4 atParam1;
layout(location = 2) in vec2 atParam2;
layout(location = 1) out vec4 etParam1;
layout(location = 0) out vec4 etParam0;

void main()
{
    gl_Position = vec4(atParam0, 1.0);
    etParam1 = vec4(atParam2, 0.0, 0.0);
    etParam0 = atParam1;
}

