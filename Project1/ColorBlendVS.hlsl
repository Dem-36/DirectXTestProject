cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(float3 position : POSITION, float4 color : COLOR)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), transform);
    vso.color = color;
    return vso;
}