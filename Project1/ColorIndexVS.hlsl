cbuffer CBuf
{
    matrix transform;
};

float4 main(float3 position : POSITION) : SV_POSITION
{
    return mul(float4(position, 1.0f), transform);
}