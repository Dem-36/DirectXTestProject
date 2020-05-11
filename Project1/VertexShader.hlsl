//ConstantBuffer
cbuffer CBuffer
{
   matrix transform; //4×4行列 row_major CPU側で転置行列に変換する？
};

float4 main(float3 position : POSITION):SV_POSITION
{
    //乗算はベクトルを左に(今回でいう位置)右に行列を指定する
    return mul(float4(position, 1.0f), transform);
}