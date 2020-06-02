#include "VertexShader.h"
#include<typeinfo>

namespace Bind {

	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);
		//���_�V�F�[�_�[�̓ǂݍ���
		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCodeBlob));
		//�쐬
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pByteCodeBlob->GetBufferPointer(),
			pByteCodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
			));
	}

	//�p�C�v���C���Ƀo�C���h����
	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	//Blob��n��(IndexBuffer��)
	ID3DBlob* VertexShader::GetByteCode() const noexcept
	{
		return pByteCodeBlob.Get();
	}
}