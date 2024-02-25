#version 450

layout(binding = 0, std140) uniform TD00000000
{
    vec4 _m0;
    vec4 _m1;
    vec4 _m2;
    vec4 _m3;
    vec4 _m4;
    vec4 _m5;
    vec4 _m6;
    vec4 _m7;
    vec4 _m8;
    vec4 _m9;
    vec4 _m10;
    vec4 _m11;
    vec2 _m12;
    float _m13;
    float _m14;
    vec2 _m15;
    float _m16;
    float _m17;
    vec2 _m18;
    float _m19;
    float _m20;
    vec2 _m21;
    float _m22;
    float _m23;
    float _m24;
    float _m25;
    float _m26;
    float _m27;
    float _m28;
    float _m29;
    float _m30;
    float _m31;
    float _m32;
    float _m33;
    float _m34;
    float _m35;
    float _m36;
    float _m37;
    float _m38;
    float _m39;
    uint _m40;
    float _m41;
    float _m42;
} uBuf0;

layout(location = 0) in vec3 atParam0;
layout(location = 1) in vec4 atParam1;
layout(location = 1) out vec4 etParam1;
layout(location = 0) out vec4 etParam0;
float vV0;

void main()
{
    vV0 = 0.0;
    if (0u != uBuf0._m40)
    {
        vV0 = uBuf0._m42 * (fma(uBuf0._m19, atParam0.z, fma(uBuf0._m16, atParam0.y, fma(uBuf0._m13, atParam0.x, uBuf0._m22))) - uBuf0._m41);
    }
    gl_Position = vec4(fma(uBuf0._m32, atParam0.z, fma(uBuf0._m28, atParam0.y, fma(uBuf0._m24, atParam0.x, uBuf0._m36))), fma(uBuf0._m33, atParam0.z, fma(uBuf0._m29, atParam0.y, fma(uBuf0._m25, atParam0.x, uBuf0._m37))), fma(uBuf0._m34, atParam0.z, fma(uBuf0._m30, atParam0.y, fma(uBuf0._m26, atParam0.x, uBuf0._m38))), fma(uBuf0._m35, atParam0.z, fma(uBuf0._m31, atParam0.y, fma(uBuf0._m27, atParam0.x, uBuf0._m39))));
    etParam1 = vec4(vV0, 0.0, 0.0, 0.0);
    etParam0 = atParam1;
}

