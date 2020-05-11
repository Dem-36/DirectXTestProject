#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include"WinInc.h"
#include<d3d11.h>
#include<wrl.h>
#include"DxgiInfoManager.h"

//DirectX11�̏������s��
class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	//��ʃN���A
	void ClearBuffer(float red, float green, float blue)noexcept;

	void DrawTriangle(float angle);

private:
#ifndef NODEBUG
	DxgiInfoManager infoManager;
#endif
	//DirectX11�̊�b�I�v�f
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	//�o�b�t�@�؂�ւ�
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	//�`�揈��
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};

#endif