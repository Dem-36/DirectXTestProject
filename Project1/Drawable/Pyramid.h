#ifndef _PYRAMID_H_
#define _PYRAMID_H_

#include"TestObject.h"

class Pyramid :public TestObject<Pyramid> {
public:
	//コンストラクタ
	Pyramid(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_int_distribution<int>& tdist);
};

#endif