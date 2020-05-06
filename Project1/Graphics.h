#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include"WinInc.h"
#include<d3d11.h>

//DirectX11�̏������s��
class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	//��ʃN���A
	void ClearBuffer(float red, float green, float blue)noexcept {
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget, color);
	}
private:
	//DirectX11�̊�b�I�v�f
	ID3D11Device* pDevice = nullptr;
	//�o�b�t�@�؂�ւ�
	IDXGISwapChain* pSwap = nullptr;
	//�`�揈��
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};

#endif