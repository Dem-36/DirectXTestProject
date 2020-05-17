#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

#include"Bindable.h"

//インデックスバッファ
class IndexBuffer :public Bindable {
public:
	//コンストラクタ
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	//バッファの情報をバインドする
	void Bind(Graphics& gfx)noexcept override;
	//頂点数を返す
	UINT GetCount()const noexcept;
protected:
	//頂点数
	UINT count;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>pIndexBuffer = nullptr;
};

#endif