//ConstantBuffer
cbuffer CBuffer
{
   row_major matrix transform; //4×4行列 row_major CPU側で転置行列に変換する？
};

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
    //乗算はベクトルを左に(今回でいう位置)右に行列を指定する
    o.position = mul(float4(v.vertex.x, v.vertex.y, 0.0f, 1.0f), transform);
    o.color = v.color;
    return o;
}