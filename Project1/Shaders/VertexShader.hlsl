//ConstantBuffer
cbuffer CBuffer
{
   matrix transform; //4�~4�s�� row_major CPU���œ]�u�s��ɕϊ�����H
};

float4 main(float3 position : POSITION):SV_POSITION
{
    //��Z�̓x�N�g��������(����ł����ʒu)�E�ɍs����w�肷��
    return mul(float4(position, 1.0f), transform);
}