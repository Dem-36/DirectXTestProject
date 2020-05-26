cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProjection;
};

struct appdata
{
    float3 vertex : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct v2f
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 position : SV_POSITION;
};

v2f main(appdata v)
{
    v2f o;
    o.worldPos = (float3) mul(float4(v.vertex, 1.0f), modelView);
    o.normal = mul(v.normal,(float3x3)modelView);
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    o.texcoord = v.texcoord;
    return o;
}