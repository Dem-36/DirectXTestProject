#ifndef _ASSTEST_H_
#define _ASSTEST_H_

#include"TestObject.h"
#include"../Bindable/ConstantBuffer.h"
#include<random>

class AssTest :public TestObject<AssTest> {
public:
	AssTest(Graphics& gfx, std::mt19937 rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,float scale);
};

#endif