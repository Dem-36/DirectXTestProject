#ifndef _BOX_H_
#define _BOX_H_

#include"DrawableBase.h"
#include<random>

class Box :public DrawableBase<Box>
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
	//更新
	void Update(float dt)noexcept override;
	//モデル行列を返す
	DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	//positional
	float r;
	float roll = 0.0f;   //x
	float pitch = 0.0f;  //y
	float yaw = 0.0f;    //z
	float theta;
	float phi;
	float chi;
	//speed (delta / 2)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	//3×3の行列
	DirectX::XMFLOAT3X3 mt;
};
#endif
