#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include"Bindable.h"

namespace Bind
{
	class Sampler :public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx)noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler = nullptr;
	};
}
#endif

