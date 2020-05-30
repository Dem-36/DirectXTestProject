#include "Sampler.h"
#include"WinExceptionMacro.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//‚±‚ê‚ðŽg—p‚·‚éê‡‚Í,AddressU,AddressV‚ðD3D11_TEXTURE_ADDRESS_BORDER‚É‚·‚é
		//sampDesc.BorderColor[0] = 0.0f;
		//sampDesc.BorderColor[1] = 1.0f;
		//sampDesc.BorderColor[2] = 1.0f;
		//sampDesc.BorderColor[3] = 0.0f;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sampDesc, pSampler.GetAddressOf()));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
}