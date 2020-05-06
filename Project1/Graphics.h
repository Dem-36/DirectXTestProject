#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include"WinInc.h"
#include<d3d11.h>

//DirectX11の処理を行う
class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	//画面クリア
	void ClearBuffer(float red, float green, float blue)noexcept {
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget, color);
	}
private:
	//DirectX11の基礎的要素
	ID3D11Device* pDevice = nullptr;
	//バッファ切り替え
	IDXGISwapChain* pSwap = nullptr;
	//描画処理
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};

#endif