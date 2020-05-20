#include "Graphics.h"
#include"WinExceptionMacro.h"
#include<cmath>
#include"imgui/imgui_impl_dx11.h"
#include"imgui/imgui_impl_win32.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	//�f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g���쐬����
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
		));

	//�o�b�N�o�b�t�@�̃n���h����ێ�����|�C���^���쐬
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	//�����_�[�^�[�Q�b�g�̎擾
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
		));

	//Depth Buffer
	//�[�x�e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE; //�[�x�e�X�g��L���ɂ���
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//�[�x�X�e���V���o�b�t�@�̏������݂��I���ɂ���
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;//�[�x�f�[�^�̔�r
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	//bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	//�[�x�X�e���V���o�b�t�@�̍쐬
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;      //�f�v�X�e�N�X�`���̕�
	descDepth.Height = 600u;     //�f�v�X�e�N�X�`���̍���
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	//create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV,&pDSV));
	//bind depth stencil view to OM
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	SetViewport();

	//init imgui d3d impl
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

void Graphics::SetViewport()
{
	//create viewport
	//�X�N���[���̑傫���Ɠ����ɂ���
	//����͕`��̈�̂悤�Ȃ���(widthmheight)
	//TopLeftX,TopLeftY�͕`��̈�̎n�܂�(����͍���)
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::BeginFrame(float red, float green, float blue) noexcept
{
	if (imguiEnabled) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

//�`�揈��
void Graphics::EndFrame()
{
	if (imguiEnabled) {
		//imgui�̕`����s��
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	HRESULT hr;
	//�o�b�t�@�̓���ւ�
	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_REMOVE_EXCEPTION(pDevice->GetDeviceRemovedReason());
		}
		else {
			GFX_THROW_INFO(hr);
		}
	}
}

void Graphics::DrawIndexed(UINT count) 
{
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
	return camera;
}

void Graphics::EnableImgui() noexcept
{
	imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept
{
	imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept
{
	return imguiEnabled;
}
