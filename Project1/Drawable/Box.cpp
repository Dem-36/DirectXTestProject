#include "Box.h"
#include"../WinExceptionMacro.h"
#include"../Bindable/BindableBase.h"
#include"../Geometry/Cube.h"
#include"../imgui/imgui.h"

using namespace Bind;

//コンストラクタ
Box::Box(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 materialColor)
	:TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	//もしBoxの情報がバインドされていないなら
	if (!IsStaticInitialized()) {
		struct Vertex {
			dx::XMFLOAT3 position;
			dx::XMFLOAT3 normal;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		//boxの法線情報を計算する
		model.SetNormalsIndependentFlat();

		//頂点バッファの作成
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		//頂点シェーダーを作成
		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		//IndexBufferで使用するため、Blob情報を取得
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		//ピクセルシェーダーを作成
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		//IndexBufferの登録(描画時に頂点数を入力するため別の方法で保存)
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		//頂点シェーダーに渡すセマンティクス情報を作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
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

	materialConstants.color = materialColor;
	//個々で色情報を持つのでstaticではバインドしない
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, materialConstants, 1u));

	//XMMatrixを3×3行列に変換する = 拡大縮小行列
	//bdist(rng)でZの大きさをランダムで指定
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}


//モデル行列を返す
DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM();
}

//Box情報を変更する
bool Box::SpawnControlWindow(int id, Graphics& gfx) noexcept
{
	using namespace std::string_literals;

	bool dirty = false;
	bool open = true;
	if (ImGui::Begin(("Box "s + std::to_string(id)).c_str(),&open))
	{ 
		ImGui::Text("Material Properties");
		const bool cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
		const bool sid =  ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", 2);
		const bool spd =  ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2);
		dirty = cd | sid | spd;

		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		//焦点を中心とした回転角度
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -180.0f,180.0f);
		//回転角度
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	}
	ImGui::End();

	if (dirty)
	{
		SyncMaterial(gfx);
	}
	return open;
}

void Box::SyncMaterial(Graphics& gfx) noexcept
{
	//一時的に定数バッファをいじる
	auto pConstPS = QueryBindable<MaterialCBuf>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, materialConstants);
}
