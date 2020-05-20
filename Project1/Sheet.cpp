#include "Sheet.h"
#include"BindableBase.h"
#include"WinExceptionMacro.h"
#include"Plane.h"
#include"Texture.h"
#include"Sampler.h"

Sheet::Sheet(Graphics& gfx, std::mt19937& rng, 
	std::uniform_real_distribution<float>& adist, 
	std::uniform_real_distribution<float>& ddist, 
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:r(rdist(rng)),droll(ddist(rng)),dpitch(ddist(rng)),
	dyaw(ddist(rng)),dphi(odist(rng)),dtheta(odist(rng)),
	dchi(odist(rng)),chi(adist(rng)),theta(adist(rng)),phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized()) {
		struct Vertex {
			dx::XMFLOAT3 position;
			struct {
				float u, v;
			}uv;
		};

		auto model = Plane::Make<Vertex>();
		//model.vertices[0].uv = { 0.0f,0.0f };
		//model.vertices[1].uv = { 1.0f,0.0f };
		//model.vertices[2].uv = { 0.0f,1.0f };
		//model.vertices[3].uv = { 1.0f,1.0f };

		model.vertices[0].uv = { 0.0f,0.0f };
		model.vertices[1].uv = { 1.0f,0.0f };
		model.vertices[2].uv = { 0.0f,1.0f };
		model.vertices[3].uv = { 1.0f,1.0f };


		//âÊëúÇÃì«Ç›çûÇ› ìoò^
		AddStaticBind(std::make_unique<Texture>(gfx, L"Resource\\img\\test.png"));
		//í∏ì_ÉfÅ[É^ï€ë∂
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		AddStaticBind(std::make_unique<Sampler>(gfx));

		auto pvs = std::make_unique<VertexShader>(gfx, L"DrawTextureVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"DrawTexturePS.cso"));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else {
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Sheet::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
