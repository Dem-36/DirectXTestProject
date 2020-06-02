#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include"Bindable.h"
#include"WICTextureLoader.h"

namespace Bind
{
	class Texture :public Bindable
	{
	public:
		Texture(Graphics& gfx, const wchar_t* path);
		void Bind(Graphics& gfx)noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView = nullptr;

	};
}
#endif

