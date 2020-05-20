#include"Pyramid.h"
#include"BindableBase.h"
#include"WinExceptionMacro.h"
#include"Cone.h"

Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)),
	dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)),
	theta(adist(rng)), phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		//頂点構造体
		struct Vertex
		{
			dx::XMFLOAT3 position;
			struct
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			} color;
		};

		//三角錐の頂点数は底辺の頂点数 + 一番上 + 底辺の真ん中
		auto model = Cone::MakeTesselated<Vertex>(4);
		//色をランダムで指定する
		std::uniform_int_distribution<int>coldistr{ 80,255 };
		std::uniform_int_distribution<int>coldistg{ 80,255 };
		std::uniform_int_distribution<int>coldistb{ 80,255 };
		for (int i = 0; i < model.vertices.size(); i++) {
			model.vertices[i].color = {
				static_cast<unsigned char>(coldistr(rng)),
				static_cast<unsigned char>(coldistg(rng)),
				static_cast<unsigned char>(coldistb(rng)) };
		}
		// deform mesh linearly
		model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorBlendVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorBlendPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Pyramid::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
