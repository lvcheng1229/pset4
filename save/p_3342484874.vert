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
    vec4 _m12;
    vec4 _m13;
    vec4 _m14;
    vec4 _m15;
    vec4 _m16;
    vec4 _m17;
    vec4 _m18;
    vec4 _m19;
    vec4 _m20;
    vec4 _m21;
    vec4 _m22;
    vec4 _m23;
    vec4 _m24;
    vec4 _m25;
    vec4 _m26;
    vec4 _m27;
    vec4 _m28;
    vec4 _m29;
    vec4 _m30;
    vec4 _m31;
    vec4 _m32;
    vec4 _m33;
    vec4 _m34;
    vec4 _m35;
    vec4 _m36;
    vec4 _m37;
    vec4 _m38;
    vec4 _m39;
    vec4 _m40;
    vec4 _m41;
    vec4 _m42;
    float _m43;
    float _m44;
    float _m45;
    float _m46;
    float _m47;
    float _m48;
    float _m49;
    float _m50;
    float _m51;
    float _m52;
    float _m53;
    float _m54;
    float _m55;
    float _m56;
    float _m57;
    float _m58;
} uBuf0;

layout(location = 0) in vec3 atParam0;
layout(location = 1) in vec2 atParam1;
layout(location = 0) out vec4 etParam0;

void main()
{
    gl_Position = vec4(fma(uBuf0._m45, atParam0.z, fma(uBuf0._m44, atParam0.y, fma(uBuf0._m43, atParam0.x, uBuf0._m46))), fma(uBuf0._m49, atParam0.z, fma(uBuf0._m48, atParam0.y, fma(uBuf0._m47, atParam0.x, uBuf0._m50))), fma(uBuf0._m53, atParam0.z, fma(uBuf0._m52, atParam0.y, fma(uBuf0._m51, atParam0.x, uBuf0._m54))), fma(uBuf0._m57, atParam0.z, fma(uBuf0._m56, atParam0.y, fma(uBuf0._m55, atParam0.x, uBuf0._m58))));
    etParam0 = vec4(atParam1, 0.0, 0.0);
}

