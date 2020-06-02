#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "Graphics.h"
#include "Bindable/ConstantBuffer.h"
#include "Drawable/SolidSphere.h"

class DirectionalLight
{
public:
	DirectionalLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow()noexcept;
	void Reset()noexcept;
	void Draw(Graphics& gfx)const noexcept;
	void Bind(Graphics& gfx)const noexcept;
private:
	struct DirectionalLightBuffer {
		alignas(16) DirectX::XMFLOAT3 position;
		alignas(16) DirectX::XMFLOAT3 lightColor;
		float intensity;
	};
private:
	DirectionalLightBuffer data;
	mutable Bind::PixelConstantBuffer<DirectionalLightBuffer> buffer;
	mutable SolidSphere lightMesh;
};

#endif