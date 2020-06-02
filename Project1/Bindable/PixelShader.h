#ifndef _PIXELSHADER_H_
#define _PIXELSHADER_H_


#include"Bindable.h"
#include"../WinExceptionMacro.h"

namespace Bind
{
	//ピクセルシェーダー作成
	class PixelShader :public Bindable
	{
	public:
		//コンストラクタ
		PixelShader(Graphics& gfx, const std::wstring& path);
		//パイプラインにバインド
		void Bind(Graphics& gfx)noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	};
}
#endif
