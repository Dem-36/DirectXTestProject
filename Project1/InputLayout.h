#ifndef _INPUTLAYOUT_H_
#define _INPUTLAYOUT_H_

#include"Bindable.h"
#include"WinExceptionMacro.h"

namespace Bind
{
	//���_�C���v�b�g���C�A�E�g
	class InputLayout :public Bindable {
	public:
		//�R���X�g���N�^
		InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShaderByteCode);
		//�p�C�v���C���Ƀo�C���h
		void Bind(Graphics& gfx)noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	};
}
#endif