#version 450

struct TDFFFFFFFF
{
    vec4 _m0;
    vec4 _m1;
    vec4 _m2;
    vec4 _m3;
    float _m4;
};

uniform TDFFFFFFFF cBuf;

uniform sampler2D SPIRV_Cross_CombineduImg0uSmp1;

layout(location = 0) in vec4 itParam0;
layout(location = 0) out vec4 etMrt0;

void main()
{
    etMrt0 = vec4(texture(SPIRV_Cross_CombineduImg0uSmp1, vec2(fma(cBuf._m4, -0.14200000464916229248046875, itParam0.x), fma(cBuf._m4, -0.094800002872943878173828125, itParam0.y))).x, texture(SPIRV_Cross_CombineduImg0uSmp1, vec2(fma(cBuf._m4, 0.0943000018596649169921875, itParam0.x), fma(cBuf._m4, -0.0582900010049343109130859375, itParam0.y))).y, texture(SPIRV_Cross_CombineduImg0uSmp1, vec2(fma(cBuf._m4, -0.0419999994337558746337890625, itParam0.x), fma(cBuf._m4, 0.06239999830722808837890625, itParam0.y))).z, 1.0);
}

