#include "IndexBuffer.h"
#include"WinExceptionMacro.h"

//コンストラクタ
//インデックスバッファの作成(描画時に必要な頂点情報)
IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
	:count((UINT)indices.size())
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd = {};

	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices.data();
	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &initData, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

//使用する頂点数を返す
UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
