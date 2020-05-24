#ifndef _BOX_H_
#define _BOX_H_

#include"TestObject.h"
#include<random>

class Box :public TestObject<Box>
{
public:
	//コンストラクタ
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 materialColor);
	//モデル行列を返す
	DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	//3×3の行列
	DirectX::XMFLOAT3X3 mt;
};
#endif
