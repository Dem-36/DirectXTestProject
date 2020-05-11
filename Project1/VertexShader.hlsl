//ConstantBuffer
cbuffer CBuffer
{
   row_major matrix transform; //4�~4�s�� row_major CPU���œ]�u�s��ɕϊ�����H
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
    //��Z�̓x�N�g��������(����ł����ʒu)�E�ɍs����w�肷��
    o.position = mul(float4(v.vertex.x, v.vertex.y, 0.0f, 1.0f), transform);
    o.color = v.color;
    return o;
}