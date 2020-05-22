cbuffer CBuf
{
    matrix model;
    matrix modelViewProjection;
}

struct appdata
{
    float3 vertex : POSITION;
};

struct v2f
{
    float4 position : SV_POSITION;
};

v2f main(appdata v)
{
    v2f o;
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    return o;
}