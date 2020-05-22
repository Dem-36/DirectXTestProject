#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include"Graphics.h"
#include"ConstantBuffer.h"
#include"SolidSphere.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow()noexcept;
	void Reset()noexcept;
	void Draw(Graphics& gfx)const noexcept;
	void Bind(Graphics& gfx)const noexcept;
private:
	struct PointLightCBuf {
		DirectX::XMFLOAT3 position;
		float padding;
	};
private:
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	//ライト用コンスタントバッファ
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
	mutable SolidSphere mesh;
};
#endif
