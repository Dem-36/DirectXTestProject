#ifndef _CONSTANTBUFFER_H_
#define _CONSTANTBUFFER_H_ 

#include"Bindable.h"
#include"WinExceptionMacro.h"

template<typename T>
class ConstantBuffer :public Bindable {
public:
	//�萔�o�b�t�@�̍X�V
	void Update(Graphics& gfx, const T& consts) {
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
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

	//�R���X�g���N�^
	ConstantBuffer(Graphics& gfx, const T& consts) {

		INFOMAN(gfx);
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(T);
		bd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pConstantBuffer));
	}

	//�R���X�g���N�^
	ConstantBuffer(Graphics& gfx) {
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
};

//���_�V�F�[�_�[�̒萔�o�b�t�@
template<typename T>
class VertexConstantBuffer :public ConstantBuffer<T> {
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx)noexcept override {
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

//�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@
template<typename T>
class PixelConstantBuffer :public ConstantBuffer<T> {
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx)noexcept override {
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};


#endif