//ライトの定数バッファ
cbuffer LightBuf
{
    float3 lightPosition;     //ライトの位置
    float3 ambient;           //環境色
    float3 diffuseColor;      //拡散反射光
    float diffuseIntensity;   //強度
    float attConst;           //距離に関係なく減衰させる
    float attLin;
    float attQuad;
};

//オブジェクトの定数バッファ
cbuffer ObjectCBuffer
{
    float3 materialColor;      //マテリアル色
    float specularIntensity;
    float specularPower;
};

struct v2f
{
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

float4 main(v2f i) : SV_TARGET
{
    //方向ベクトルを求める(距離関係なくライトの効果を与える = DirectionalLight)
    //const float3 vToL = normalize(lightPosition - i.worldPosition);
    //float3 diffuse = diffuseColor * diffuseIntensity * max(0.0f, dot(vToL, i.normal));
    //return float4(diffuse, 1.0f);
    const float3 vToL = lightPosition - i.worldPosition;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    //diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    //diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, i.normal));
    
    //specular
    const float3 w = i.normal * dot(vToL, i.normal);
    //refrect
    const float3 r = w * 2.0f - vToL;
    //calculate specular intensity based on angle between viewing vector
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * 
    pow(max(0.0f, dot(normalize(-r), normalize(i.worldPosition))), specularPower);
    
    return float4(saturate(diffuse + ambient + specular) * materialColor, 1.0f);

}