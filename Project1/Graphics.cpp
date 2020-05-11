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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	//デバイス、スワップチェーン、コンテキストを作成する
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

	//バックバッファのハンドルを保持するポインタを作成
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	//レンダーターゲットの取得
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
		));
}

//描画処理
void Graphics::EndFrame()
{
	HRESULT hr;
	//バッファの入れ替え
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
	//ベクトルと行列の掛け算
	auto result = dx::XMVector3Transform(v,dx::XMMatrixScaling(1.5f,0.0f,0.0f));
	auto xx = dx::XMVectorGetX(result);
	HRESULT hr;

	//頂点情報構造体
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
	//パイプラインに頂点情報をバインドする
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

	//Z軸回転行列(シェーダーに渡すときに転置行列にしないといけない Transpose)
	const ConstantBuffer cb = {
		{
			//行列同士の掛け算を行う(回転行列 * 拡大縮小行列)
			//dx::XMMatrixMultiply(
			//dx::XMMatrixRotationZ(angle),
			//	dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f))

			//通常の掛け算も行える
			//dx::XMMatrixTranspose(
			//dx::XMMatrixRotationZ(angle) * 
			//	dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f))

			//回転、拡大縮小、移動
			dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle)*
				dx::XMMatrixScaling(3.0f / 4.0f,1.0f,1.0f) *
				dx::XMMatrixTranslation(x,y,0.0f))


		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//定数を指定する場合、DYNAMICを選ぶ？(動的バッファ)
	//使用する場合、CPUAccessFlagsを変更する
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;//定数バッファが配列でないので構造バイトは0
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
	//スクリーンの大きさと同じにする
	//これは描画領域のようなもの(widthmheight)
	//TopLeftX,TopLeftYは描画領域の始まり(現状は左上)
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
