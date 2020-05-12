#include "InputLayout.h"

//�R���X�g���N�^
InputLayout::InputLayout(Graphics& gfx, 
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
	ID3DBlob* pVertexShaderByteCode)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout
		));
}

//�p�C�v���C���Ƀo�C���h
void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
