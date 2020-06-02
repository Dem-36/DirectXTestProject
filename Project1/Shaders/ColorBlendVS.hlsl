cbuffer CBuf
{
    matrix model;
    matrix modelViewProjection;
};

struct appdata
{
    float3 vertex : POSITION;
    float4 color : COLOR;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(appdata v)
{
    VSOut vso;
    vso.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    vso.color = v.color;
    return vso;
}