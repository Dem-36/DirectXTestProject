#ifndef _VERTEXSHADER_H_
#define _VERTEXSHADER_H_

#include"Bindable.h"
#include"../WinExceptionMacro.h"

namespace Bind {
	//���_�V�F�[�_�[
	class VertexShader :public Bindable
	{
	public:
		//�R���X�g���N�^
		VertexShader(Graphics& gfx, const std::wstring& path);
		//�p�C�v���C���Ƀo�C���h����
		void Bind(Graphics& gfx)noexcept override;
		//Blob���擾 InputLayout�Ŏg�p
		ID3DBlob* GetByteCode()const noexcept;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	};
}
#endif
