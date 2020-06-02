#ifndef _TESTOBJECT_H_
#define _TESTOBJECT_H_

#include"DrawableBase.h"
#include<random>
#include"../Utility/WinMath.h"

template<class T>
class TestObject :public DrawableBase<T> {
public:
	TestObject(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist)
		:r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)),
		dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)),
		dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng)) {

	}

	void Update(float dt)noexcept {
		//オブジェクトの回転角度
		roll = wrap_angle(roll + droll * dt);
		pitch = wrap_angle(pitch + dpitch * dt);
		yaw = wrap_angle(yaw + dyaw * dt);
		//焦点からの回転角度
		theta = wrap_angle(theta + dtheta * dt);
		phi = wrap_angle(phi + dphi * dt);
		chi = wrap_angle(chi + dchi * dt);
	}
	DirectX::XMMATRIX GetTransformXM()const noexcept {
		//XMLoadFloat3×3 = 3×3行列をXMMATRIXに変換する mtはモデル行列
        //XMMatrixRotationRollPitchYaw = 指定されたオイラー角に基づいて回転行列を作成

		namespace dx = DirectX;
		//1:オブジェクトの中心に沿って回転
		//2:焦点に向かって移動(x)
		//3:焦点に沿って回転
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
protected:
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
};

#endif