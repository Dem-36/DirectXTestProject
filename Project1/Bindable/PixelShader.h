#ifndef _PIXELSHADER_H_
#define _PIXELSHADER_H_


#include"Bindable.h"
#include"../WinExceptionMacro.h"

namespace Bind
{
	//�s�N�Z���V�F�[�_�[�쐬
	class PixelShader :public Bindable
	{
	public:
		//�R���X�g���N�^
		PixelShader(Graphics& gfx, const std::wstring& path);
		//�p�C�v���C���Ƀo�C���h
		void Bind(Graphics& gfx)noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	};
}
#endif
