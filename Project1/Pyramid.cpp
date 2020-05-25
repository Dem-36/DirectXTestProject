#include"Pyramid.h"
#include"BindableBase.h"
#include"WinExceptionMacro.h"
#include"Cone.h"
#include<array>

Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& tdist)
	:TestObject(gfx,rng,adist,ddist,odist,rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		//頂点構造体
		struct Vertex
		{
			dx::XMFLOAT3 position;
			dx::XMFLOAT3 normal;
			std::array<char, 4> color;
		};

		const auto tesselation = tdist(rng);
		//三角錐の頂点数は底辺の頂点数 + 一番上 + 底辺の真ん中
		auto model = Cone::MakeTesselatedIndependentFaces<Vertex>(tesselation);
		for (auto& v : model.vertices) {
			v.color = { (char)10,(char)10,(char)255 };
		}
		for (int i = 0; i < tesselation; i++) {
			model.vertices[i * 3].color = { (char)255,(char)10,(char)10 };
		}

		// deform mesh linearly
		model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"BlendPhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"BlendPhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//bytewidthは16の倍数でないといけないため、
		//paddingで調整する
		struct PSMaterialConstant {
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		}colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst,1u));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}