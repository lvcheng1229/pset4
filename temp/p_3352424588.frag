#version 450

struct TDFFFFFFFF
{
    uint _m0;
    float _m1;
    vec2 _m2;
    float _m3;
    float _m4;
    float _m5;
    float _m6;
    uint _m7;
    float _m8;
};

uniform TDFFFFFFFF cBuf;

uniform sampler2D SPIRV_Cross_CombineduImg0uSmp1;

layout(location = 1) in vec4 itParam1;
layout(location = 0) in vec4 itParam0;
layout(location = 2) in vec4 itParam2;
layout(location = 0) out vec4 etMrt0;
float vV2;
float vV3;
float vV4;
bool vS16;

void main()
{
    vS16 = true;
    vec4 _60 = texture(SPIRV_Cross_CombineduImg0uSmp1, vec2(itParam1.xy));
    float _70 = _60.x * itParam0.x;
    vV2 = _70;
    float _71 = itParam0.y * _60.y;
    vV3 = _71;
    float _72 = itParam0.z * _60.z;
    vV4 = _72;
    float _73 = itParam0.w * _60.w;
    if (0u != cBuf._m7)
    {
        bool _77 = cBuf._m8 >= _73;
        vS16 = !_77;
        if (!_77)
        {
            discard;
        }
    }
    if (0u != cBuf._m0)
    {
        float _90 = clamp(max(0.0, itParam2.x), 0.0, 1.0);
        vV2 = fma(cBuf._m3 - _70, _90, _70);
        vV3 = fma(cBuf._m4 - _71, _90, _71);
        vV4 = fma(cBuf._m5 - _72, _90, _72);
    }
    if (vS16)
    {
        etMrt0 = vec4(vV2, vV3, vV4, _73);
    }
    else
    {
        discard;
    }
}

