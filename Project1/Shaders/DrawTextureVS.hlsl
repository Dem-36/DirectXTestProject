cbuffer CBuf
{
    matrix model;
    matrix modelViewProjection;
};

struct appdata
{
    float3 vertex : POSITION;
    float2 texcoord : TEXCOORD;
};

struct v2f
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

v2f main(appdata v)
{
    v2f o;
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    o.uv = v.texcoord;
    return o;
}