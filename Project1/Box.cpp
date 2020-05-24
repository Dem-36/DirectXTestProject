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
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 materialColor)
	:TestObject(gfx,rng,adist,ddist,odist,rdist)
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

	struct PSMaterialConstant {
		alignas(16) dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
	}colorConst;

	colorConst.color = materialColor;
	//個々で色情報を持つのでstaticではバインドしない
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

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
