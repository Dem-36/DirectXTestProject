struct v2f
{
    float4 position : SV_Position;
    float3 color : COLOR;
};

float4 main(v2f i) : SV_TARGET
{
    return float4(i.color, 1.0f);
}
