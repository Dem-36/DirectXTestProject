#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	//�X���b�v�`�F�[���ɕK�v�ȍ\����
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;//�𑜓x�̕�
	sd.BufferDesc.Height = 0;//�𑜓x�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//�A���`�G�C���A�X�����ɂ���
	sd.SampleDesc.Count = 1;    //�s���Ă���̂ŃJ�E���g��1�ɐݒ�
	sd.SampleDesc.Quality = 0;  //�A���`�G�C���A�X�o�b�t�@�[���g�p�ł��Ȃ��Ȃ�
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//�f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g���쐬����
	D3D11CreateDeviceAndSwapChain(
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
		);

	//�o�b�N�o�b�t�@�̃n���h����ێ�����|�C���^���쐬
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget
		);
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

//�`�揈���H�H
void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}
