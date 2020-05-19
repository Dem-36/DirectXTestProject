struct v2f
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(v2f i) : SV_TARGET
{
    float3 pixelColor = objTexture.Sample(objSamplerState, i.uv);
    return float4(pixelColor, 1.0f);
}