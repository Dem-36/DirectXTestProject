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
	void Bind(Graphics& gfx,DirectX::FXMMATRIX viewMatrix)const noexcept;
private:
	struct PointLightCBuf {
		alignas(16)	DirectX::XMFLOAT3 position;         //ライトの位置
		alignas(16)	DirectX::XMFLOAT3 ambient;          //
		alignas(16)	DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	//ライト用コンスタントバッファ
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
	mutable SolidSphere mesh;
};
#endif
