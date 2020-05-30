#ifndef _INPUTLAYOUT_H_
#define _INPUTLAYOUT_H_

#include"Bindable.h"
#include"WinExceptionMacro.h"

namespace Bind
{
	//頂点インプットレイアウト
	class InputLayout :public Bindable {
	public:
		//コンストラクタ
		InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShaderByteCode);
		//パイプラインにバインド
		void Bind(Graphics& gfx)noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	};
}
#endif