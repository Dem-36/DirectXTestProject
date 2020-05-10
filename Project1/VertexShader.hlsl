struct appdata
{
    float4 vertex : POSITION;
    float4 color : COLOR;
};

struct v2f
{
    float4 position : SV_Position;
    float3 color : COLOR;
};

v2f main(appdata v)
{
    v2f o;
    o.position = float4(v.vertex.x, v.vertex.y, 0.0f, 1.0f);
    o.color = v.color;
    return o;
}