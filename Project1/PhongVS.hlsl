cbuffer CBuf
{
    matrix modelView;                //���f���s��
    matrix modelViewProjection;  //MVP�s��
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
    //modeiView��������ƃJ��������Ƃ����ʒu�ɂȂ�
    o.worldPosition = (float3)mul(float4(v.vertex, 1.0f), modelView);
    //w�̒l�͕s�v
    o.normal = mul(v.normal, (float3x3) modelView);
    //MVP���W
    o.position = mul(float4(v.vertex, 1.0f), modelViewProjection);
    return o;
}