#include "Texture.h"
#include"WinExceptionMacro.h"

Texture::Texture(Graphics& gfx,const wchar_t* path)
{
	INFOMAN(gfx);

	//テクスチャの読み込み
	GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(
		GetDevice(gfx),
		path,
		nullptr,
		pTextureView.GetAddressOf()));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
}
