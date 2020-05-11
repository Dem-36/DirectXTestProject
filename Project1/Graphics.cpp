#include "Graphics.h"
#include"WinExceptionMacro.h"
#include<d3dcompiler.h>
#include<cmath>
#include<DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

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
}

//�`�揈��
void Graphics::EndFrame()
{
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

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::DrawTriangle(float angle,float x,float y)
{
	dx::XMVECTOR v = dx::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
	//�x�N�g���ƍs��̊|���Z
	auto result = dx::XMVector3Transform(v,dx::XMMatrixScaling(1.5f,0.0f,0.0f));
	auto xx = dx::XMVectorGetX(result);
	HRESULT hr;

	//���_���\����
	struct Vertex {
		struct {
			float x, y;
		}position;
		struct {
			unsigned char r, g, b, a;
		}color;
	};

	Vertex vertices[] = {
	   {0.0f,0.5f,255,0,0,0},
	   {0.5f,-0.5f,0,255,0,0},
	   {-0.5f,-0.5f,0,0,255,0},
	   {-0.3f,0.3f,0,255,0,0},
	   {0.3f,0.3f,0,255,0,0},
	   {0.0f,-1.0f,255,0,0,0},
	};

	vertices[0].color.g = 255;
	//�p�C�v���C���ɒ��_�����o�C���h����
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer));

	//bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	//create index buffer
	const unsigned short indices[] = {
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};

	//create index buffer
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &iInitData, &pIndexBuffer));

	//bind index buffer
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//create constant buffer for transformation matrix
	struct ConstantBuffer {
		dx::XMMATRIX transform;
	};

	//Z����]�s��(�V�F�[�_�[�ɓn���Ƃ��ɓ]�u�s��ɂ��Ȃ��Ƃ����Ȃ� Transpose)
	const ConstantBuffer cb = {
		{
			//�s�񓯎m�̊|���Z���s��(��]�s�� * �g��k���s��)
			//dx::XMMatrixMultiply(
			//dx::XMMatrixRotationZ(angle),
			//	dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f))

			//�ʏ�̊|���Z���s����
			//dx::XMMatrixTranspose(
			//dx::XMMatrixRotationZ(angle) * 
			//	dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f))

			//��]�A�g��k���A�ړ�
			dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle)*
				dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f) *
				dx::XMMatrixTranslation(x,y,0.0f))


		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//�萔���w�肷��ꍇ�ADYNAMIC��I�ԁH(���I�o�b�t�@)
	//�g�p����ꍇ�ACPUAccessFlags��ύX����
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;//�萔�o�b�t�@���z��łȂ��̂ō\���o�C�g��0
	D3D11_SUBRESOURCE_DATA csb = {};
	csb.pSysMem = &cb;
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csb, &pConstantBuffer));

	//bind constnat buffer
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
		));

	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
		));

	//bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	//input layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
		));

	//bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	//bind render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	//set primitive topology to triangle list (groups of 3 vertices)
	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}