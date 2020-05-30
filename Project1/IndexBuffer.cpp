#include "IndexBuffer.h"
#include"WinExceptionMacro.h"

namespace Bind
{
	//コンストラクタ
	//インデックスバッファの作成(描画時に必要な頂点情報)
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
		:count((UINT)indices.size())
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC ibd = {};

		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;               //パイプラインのバインド先を指定
		ibd.Usage = D3D11_USAGE_DEFAULT;                       //バッファでのアクセス方法指定
		ibd.CPUAccessFlags = 0u;                               //リソースに対してのCPUアクセスの方法指定
		ibd.MiscFlags = 0u;                                    //その他オプション
		ibd.ByteWidth = UINT(count * sizeof(unsigned short));  //バッファのサイズ指定
		ibd.StructureByteStride = sizeof(unsigned short);      //構造体のサイズ バッファを構造体として確保する場合に使用

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = indices.data();                     //バッファを初期化するデータを指定
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &initData, &pIndexBuffer));
	}

	//パイプラインにバインドする
	void IndexBuffer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	//使用する頂点数を返す
	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}