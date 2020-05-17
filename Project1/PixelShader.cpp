#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);
	//�V�F�[�_�[�ǂݍ��݂Ɏg�p
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	//�s�N�Z���V�F�[�_�[�̍쐬
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
		));
}

//�p�C�v���C���Ƀo�C���h����
void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
