cbuffer CBuf
{
    float4 color;
};

struct v2f
{
    float4 position : SV_POSITION;
};

float4 main(v2f i) : SV_TARGET
{
    return color;
}