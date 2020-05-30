#include "VertexShader.h"
#include<typeinfo>

namespace Bind {

	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);
		//頂点シェーダーの読み込み
		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCodeBlob));
		//作成
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pByteCodeBlob->GetBufferPointer(),
			pByteCodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
			));
	}

	//パイプラインにバインドする
	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	//Blobを渡す(IndexBufferへ)
	ID3DBlob* VertexShader::GetByteCode() const noexcept
	{
		return pByteCodeBlob.Get();
	}
}