#include"AssTest.h"
#include"../Bindable/BindableBase.h"
#include"../WinExceptionMacro.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"../Vertex.h"

using namespace Bind;

AssTest::AssTest(Graphics& gfx, std::mt19937 rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	DirectX::XMFLOAT3 material, float scale)
	:TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized()) {

		using Dxv::VertexLayout;
		Dxv::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			));

		//モデルの読み込み
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("Models\\suzanne.obj",
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		//単一メッシュと想定して取得
		const auto pMesh = pModel->mMeshes[0];

		//頂点情報の取得
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
			vbuf.EmplaceBack(
				dx::XMFLOAT3{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale },
				*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
				);
		}

		//インデックス情報の取得
		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		//頂点バッファ生成
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vbuf));
		//インデックスバッファ生成
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		//頂点シェーダー生成
		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		//InputLayoutで使用するためにBlob取得
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		AddStaticBind(std::make_unique<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//ピクセル定数バッファを定義
		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} materialConst;
		materialConst.color = material;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, materialConst, 1u));
	}
	else {
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
