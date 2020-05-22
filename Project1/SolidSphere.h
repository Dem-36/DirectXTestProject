#ifndef _SOLIDSPHERE_H_
#define _SOLIDSPHERE_H_

#include"DrawableBase.h"

class SolidSphere:public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt)noexcept override;
	void SetPosition(DirectX::XMFLOAT3 position)noexcept;
	DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	DirectX::XMFLOAT3 position = { 1.0f,1.0f,1.0f };
};

#endif