#ifndef _CONSTANTBUFFER_H_
#define _CONSTANTBUFFER_H_ 

#include"Bindable.h"
#include"../WinExceptionMacro.h"

namespace Bind
{
	//定数バッファ(渡す値が違う場合あるためテンプレート)
	template<typename T>
	class ConstantBuffer :public Bindable {
	public:
		//定数バッファの更新
		void Update(Graphics& gfx, const T& consts) {
			INFOMAN(gfx);

			//Mapでリソースのデータを取得する構造体
			D3D11_MAPPED_SUBRESOURCE msr;
			//Mapを使用してバッファにアクセスする
			GFX_THROW_INFO(GetContext(gfx)->Map(
				pConstantBuffer.Get(),
				0u,
				D3D11_MAP_WRITE_DISCARD,
				0u,
				&msr
				));
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
		}

		//コンストラクタ
		ConstantBuffer(Graphics& gfx, const T& consts, UINT slot = 0u)
			:slot(slot)
		{

			INFOMAN(gfx);
			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0u;
			bd.ByteWidth = sizeof(T);                   //サイズは必ず16の倍数でないといけない
			bd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = &consts;
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pConstantBuffer));
		}

		//コンストラクタ
		ConstantBuffer(Graphics& gfx, UINT slot = 0u)
			:slot(slot)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0u;
			bd.ByteWidth = sizeof(T);
			bd.StructureByteStride = 0u;
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, nullptr, &pConstantBuffer));
		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer = nullptr;
		UINT slot; //定数バッファのスロット(デフォルトは0)
	};

	//頂点シェーダーの定数バッファ
	template<typename T>
	class VertexConstantBuffer :public ConstantBuffer<T> {
		using ConstantBuffer<T>::pConstantBuffer;
		using ConstantBuffer<T>::slot;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<T>::ConstantBuffer;
		void Bind(Graphics& gfx)noexcept override {
			GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};

	//ピクセルシェーダーの定数バッファ
	template<typename T>
	class PixelConstantBuffer :public ConstantBuffer<T> {
		using ConstantBuffer<T>::pConstantBuffer;
		using ConstantBuffer<T>::slot;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<T>::ConstantBuffer;
		void Bind(Graphics& gfx)noexcept override {
			GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};
}
#endif