#include "InputLayout.h"

//コンストラクタ
InputLayout::InputLayout(Graphics& gfx, 
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
	ID3DBlob* pVertexShaderByteCode)
{
	INFOMAN(gfx);

	//頂点シェーダーに渡す入力情報を作成
	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout
		));
}

//パイプラインにバインド
void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
