#include"AssTest.h"
#include"BindableBase.h"
#include"WinExceptionMacro.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

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

		struct Vertex {
			dx::XMFLOAT3 position;
			dx::XMFLOAT3 normal;
		};

		//���f���̓ǂݍ���
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("Models\\suzanne.obj",
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		//�P�ꃁ�b�V���Ƒz�肵�Ď擾
		const auto pMesh = pModel->mMeshes[0];

		//���_���̎擾
		std::vector<Vertex>vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
			vertices.push_back({
				{pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale},
				*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
				});
		}

		//�C���f�b�N�X���̎擾
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

		//���_�o�b�t�@����
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		//�C���f�b�N�X�o�b�t�@����
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		//���_�V�F�[�_�[����
		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		//InputLayout�Ŏg�p���邽�߂�Blob�擾
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//�s�N�Z���萔�o�b�t�@���`
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