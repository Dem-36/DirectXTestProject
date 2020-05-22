cbuffer LightBuf
{
    float3 lightPosition;     //���C�g�̈ʒu
    float3 materialColor;     //�}�e���A���̐F
    float3 ambient;           //���F
    float3 diffuseColor;      //�g�U���ˌ�
    float diffuseIntensity;   //���x
    float attConst;           //�����Ɋ֌W�Ȃ�����������
    float attLin;
    float attQuad;
};

struct v2f
{
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

float4 main(v2f i) : SV_TARGET
{
    //�����x�N�g�������߂�
    const float3 vToL = normalize(lightPosition - i.worldPosition);
    float3 diffuse = diffuseColor * diffuseIntensity * max(0.0f, dot(vToL, i.normal));
    return float4(diffuse, 1.0f);
    //const float distToL = length(vToL);
    //const float3 dirToL = vToL / distToL;
    ////diffuse attenuation
    //const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    ////diffuse intensity
    //const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, i.normal));
    //return float4(saturate(diffuse + ambient) * materialColor, 1.0f);

}