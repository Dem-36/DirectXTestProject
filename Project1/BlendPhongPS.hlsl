//���C�g�̒萔�o�b�t�@
cbuffer LightBuf
{
    float3 lightPosition; //���C�g�̈ʒu
    float3 ambient; //���F
    float3 diffuseColor; //�g�U���ˌ�
    float diffuseIntensity; //���x
    float attConst; //�����Ɋ֌W�Ȃ�����������
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
};

struct v2f
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

float4 main(v2f i) : SV_Target
{
    //���C�g�x�N�g�������߂�
    const float3 vToL = lightPosition - i.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, i.normal));
	// reflected light vector
    const float3 w = i.normal * dot(vToL, i.normal);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(i.worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient + specular) * i.color), 1.0f);
}