#include "Graphics.h"
#include"WinExceptionMacro.h"

#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	//スワップチェーンに必要な構造体
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;//解像度の幅
	sd.BufferDesc.Height = 0;//解像度の高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//アンチエイリアス処理について
	sd.SampleDesc.Count = 1;    //行っているのでカウントを1に設定
	sd.SampleDesc.Quality = 0;  //アンチエイリアスバッファーが使用できなくなる
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;
	//デバイス、スワップチェーン、コンテキストを作成する
	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
		));

	//バックバッファのハンドルを保持するポインタを作成
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_FAILED(pSwap->GetBuffer(
		0,
		__uuidof(ID3D11Resource), 
		reinterpret_cast<void**>(&pBackBuffer)));

	//レンダーターゲットの取得
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget
		));
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pTarget != nullptr)
		pTarget->Release();
	if (pSwap != nullptr)
		pSwap->Release();
	if (pContext != nullptr)
		pContext->Release();
	if (pDevice != nullptr)
		pDevice->Release();
}

//描画処理
void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_REMOVED_EXPRCT(pDevice->GetDeviceRemovedReason());
		}
		else {
			GFX_THROW_FAILED(hr);
		}
	}
}
