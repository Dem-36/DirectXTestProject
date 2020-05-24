cbuffer CBuf
{
    matrix modelView;                //モデル行列
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
    //modeiViewをかけるとカメラを基準とした位置になる
    o.worldPosition = (float3)mul(float4(v.vertex, 1.0f), modelView);
    //wの値は不要
    o.normal = mul(v.normal, (float3x3) modelView);
    //MVP座標
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    return o;
}