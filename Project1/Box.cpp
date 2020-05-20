#include "Box.h"
#include"WinExceptionMacro.h"
#include"BindableBase.h"
#include"Sphere.h"
#include"Cube.h"

//コンストラクタ
Box::Box(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)),
	dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)),
	theta(adist(rng)), phi(adist(rng))
{
	namespace dx = DirectX;

	//もしBoxの情報がバインドされていないなら
	if (!IsStaticInitialized()) {
		struct Vertex {
			dx::XMFLOAT3 position;
		};

		auto model = Cube::Make<Vertex>();

		//頂点バッファの作成
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	    //頂点シェーダーを作成
		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
		//IndexBufferで使用するため、Blob情報を取得
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		//ピクセルシェーダーを作成
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));

		//IndexBufferの登録(描画時に頂点数を入力するため別の方法で保存)
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct PixelShaderConstants {
			struct {
				float r, g, b, a;
			}face_colors[8];
		};

		//ピクセルシェーダーに渡す定数バッファ作成
		const PixelShaderConstants cb2 = {
			{
				{1.0f,1.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{1.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,0.0f,1.0f},
				{0.0f,1.0f,1.0f},
				{0.0f,0.0f,0.0f},
 			}
		};

		//ピクセルシェーダーの定数バッファ登録
		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		//頂点シェーダーに渡すセマンティクス情報を作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		//プリミティブ指定
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else {
		//インデックスバッファの情報を取得する
		SetIndexFromStatic();
	}

	//すべてのBoxに個別の情報があるため
	//(定数バッファは共通だが、GetTransformXM()の中身がそれぞれ違うため)
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	//XMMatrixを3×3行列に変換する = 拡大縮小行列
	//bdist(rng)でZの大きさをランダムで指定
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

//モデル行列を返す
DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	//XMLoadFloat3×3 = 3×3行列をXMMATRIXに変換する mtはモデル行列
	//XMMatrixRotationRollPitchYaw = 指定されたオイラー角に基づいて回転行列を作成
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
