cbuffer CBuf
{
    matrix model;                //モデル行列
    matrix modelViewProjection;  //MVP行列
};

struct appdata
{
    float3 vertex : POSITION;
    float3 normal : NORMAL;
};

struct v2f
{
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float4 position : SV_POSITION;
};

v2f main(appdata v)
{
    v2f o;
    o.worldPosition = (float3)mul(float4(v.vertex, 1.0f), model);
    //wの値は不要
    o.normal = mul(v.normal, (float3x3) model);
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    return o;
}