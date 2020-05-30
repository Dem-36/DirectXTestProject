#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include"WinInc.h"
#include<d3d11.h>
#include<wrl.h>
#include"DxgiInfoManager.h"
#include<d3dcompiler.h>
#include<memory>
#include<DirectXMath.h>

namespace Bind {
	class Bindable;
}

//DirectX11の処理を行う
class Graphics
{
	friend Bind::Bindable;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void SetViewport();
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void BeginFrame(float red, float green, float blue)noexcept;
	void EndFrame();
	void DrawIndexed(UINT count);
	
	//カメラ
	void SetCamera(DirectX::FXMMATRIX cam)noexcept;
	DirectX::XMMATRIX GetCamera()const noexcept;

	//Imgui
	void EnableImgui()noexcept;
	void DisableImgui()noexcept;
	bool IsImguiEnabled()const noexcept;

private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
	bool imguiEnabled = true;
#ifndef NODEBUG
	DxgiInfoManager infoManager;
#endif
	//DirectX11の基礎的要素
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	//バッファ切り替え
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	//描画処理
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
};

#endif