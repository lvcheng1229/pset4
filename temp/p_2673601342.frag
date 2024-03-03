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

layout(location = 0) in vec4 itParam0;
layout(location = 1) in vec4 itParam1;
layout(location = 0) out vec4 etMrt0;
float vV5;
float vV4;
float vV3;
bool vS8;

void main()
{
    vS8 = true;
    if (0u != cBuf._m7)
    {
        bool _50 = cBuf._m8 >= itParam0.w;
        vS8 = !_50;
        if (!_50)
        {
            discard;
        }
    }
    vV3 = itParam0.z;
    vV4 = itParam0.y;
    vV5 = itParam0.x;
    if (0u != cBuf._m0)
    {
        float _66 = clamp(max(0.0, itParam1.x), 0.0, 1.0);
        vV5 = fma(cBuf._m3 - itParam0.x, _66, itParam0.x);
        vV4 = fma(cBuf._m4 - itParam0.y, _66, itParam0.y);
        vV3 = fma(cBuf._m5 - itParam0.z, _66, itParam0.z);
    }
    if (vS8)
    {
        etMrt0 = vec4(vV5, vV4, vV3, itParam0.w);
    }
    else
    {
        discard;
    }
}

