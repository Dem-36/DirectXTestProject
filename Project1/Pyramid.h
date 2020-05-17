#ifndef _PYRAMID_H_
#define _PYRAMID_H_

#include"DrawableBase.h"
#include<random>

class Pyramid :public DrawableBase<Pyramid> {
public:
	//コンストラクタ
	Pyramid(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	//更新
	void Update(float dt)noexcept override;
	//モデル行列を返す
	DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	//positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	//speed (delta / s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};

#endif