#ifndef _CONSTANTBUFFER_H_
#define _CONSTANTBUFFER_H_ 

#include"Bindable.h"
#include"WinExceptionMacro.h"

//�萔�o�b�t�@(�n���l���Ⴄ�ꍇ���邽�߃e���v���[�g)
template<typename T>
class ConstantBuffer :public Bindable {
public:
	//�萔�o�b�t�@�̍X�V
	void Update(Graphics& gfx, const T& consts) {
		INFOMAN(gfx);

		//Map�Ń��\�[�X�̃f�[�^���擾����\����
		D3D11_MAPPED_SUBRESOURCE msr;
		//Map���g�p���ăo�b�t�@�ɃA�N�Z�X����
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
	ConstantBuffer(Graphics& gfx, const T& consts, UINT slot = 0u)
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

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pConstantBuffer));
	}

	//�R���X�g���N�^
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
	UINT slot; //�萔�o�b�t�@�̃X���b�g(�f�t�H���g��0)
};

//���_�V�F�[�_�[�̒萔�o�b�t�@
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

//�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@
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


#endif