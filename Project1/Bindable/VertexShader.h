#ifndef _VERTEXSHADER_H_
#define _VERTEXSHADER_H_

#include"Bindable.h"
#include"../WinExceptionMacro.h"

namespace Bind {
	//頂点シェーダー
	class VertexShader :public Bindable
	{
	public:
		//コンストラクタ
		VertexShader(Graphics& gfx, const std::wstring& path);
		//パイプラインにバインドする
		void Bind(Graphics& gfx)noexcept override;
		//Blobを取得 InputLayoutで使用
		ID3DBlob* GetByteCode()const noexcept;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	};
}
#endif
