#ifndef _MESH_H_
#define _MESH_H_

#include"Drawable/DrawableBase.h"
#include"Vertex.h"
#include"Bindable/BindableBase.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class Mesh :public DrawableBase<Mesh> {
public:
	//�R���X�g���N�^
	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>>bindPtrs);
	//�`�揈��
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform)const noexcept;
	DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node {
	//Model��Node��private���g�p�ł���
	friend class Model;
public:
	Node(const std::string& name,std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept;
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	void ShowTree()const noexcept;
private:
	void AddChild(std::unique_ptr<Node> pChild) noexcept;
private:
	std::string nodeName;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class Model {
public:
	Model(Graphics& gfx, const std::string fileName);
	void Draw(Graphics& gfx) const;
	void ShowWindow(const char* windowName = nullptr)noexcept;
	~Model()noexcept;
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(const aiNode& node)noexcept;
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};

//
//class Mesh :public DrawableBase<Mesh> {
//public:
//	//�R���X�g���N�^
//	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindPtrs) {
//		//���ʐݒ肪����Ă��Ȃ��Ȃ�
//		if (!IsStaticInitialized()) {
//			//�v���~�e�B�u�w��
//			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//		}
//
//		for (auto& pb : bindPtrs) {
//			//�L���X�g��IndexBuffer��T��
//			if (auto pi = dynamic_cast<IndexBuffer*>(pb.get())) {
//				AddIndexBuffer(std::unique_ptr<IndexBuffer>{pi});
//				pb.release();
//			}
//			else {
//				AddBind(std::move(pb));
//			}
//		}
//
//		AddBind(std::make_unique<TransformCbuf>(gfx, *this));
//	}
//
//	//�`�揈��
//	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform)const noexcept {
//		DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
//		//�p�����Draw�w��
//		Drawable::Draw(gfx);
//	}
//
//	DirectX::XMMATRIX GetTransformXM()const noexcept override {
//		return DirectX::XMLoadFloat4x4(&transform);
//	}
//
//private:
//	//const���Œl��������(�g��k���A��]�A�ړ��s�� = ���f���s��)
//	mutable DirectX::XMFLOAT4X4 transform;
//};
//
//class Node {
//	//Model��Node��private���g�p�ł���
//	friend class Model;
//public:
//	Node(std::vector<Mesh*>meshPtrs, const DirectX::XMMATRIX& transform)noexcept
//		:meshPtrs(std::move(meshPtrs))
//	{
//		DirectX::XMStoreFloat4x4(&this->transform, transform);
//	}
//	void Darw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform)const noexcept {
//		//XMFLOAT4X4��MXMATRIX�ɕϊ�
//		const auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
//		//���g�̃��b�V����`��
//		for (const auto pm : meshPtrs) {
//			pm->Draw(gfx, built);
//		}
//		//�q�̃��b�V���`��
//		for (const auto& pc : childPtrs) {
//			pc->Darw(gfx, built);
//		}
//	}
//
//private:
//	//�q�̒ǉ�
//	void AddChild(std::unique_ptr<Node>pChild)noexcept {
//		assert(pChild);
//		childPtrs.push_back(std::move(pChild));
//	}
//private:
//	std::vector<std::unique_ptr<Node>> childPtrs;
//	std::vector<Mesh*> meshPtrs;
//	DirectX::XMFLOAT4X4 transform;
//};
//
//class Model {
//public:
//	Model(Graphics& gfx, const std::string fileName) {
//		Assimp::Importer importer;
//		//���f���f�[�^�ǂݍ���
//		const auto modelData = importer.ReadFile(fileName.c_str(),
//			aiProcess_Triangulate |
//			aiProcess_JoinIdenticalVertices);
//
//		for (size_t i = 0; i < modelData->mNumMeshes; i++) {
//			meshPtrs.push_back(ParseMesh(gfx, *modelData->mMeshes[i]));
//		}
//
//		pRoot = ParseNode(*modelData->mRootNode);
//	}
//
//	//���b�V���̏����쐬
//	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh) {
//		namespace dx = DirectX;
//		using WinVertex::VertexLayout;
//
//		WinVertex::VertexBuffer vbuf(std::move(
//			VertexLayout{}
//			.Append(VertexLayout::Position3D)
//			.Append(VertexLayout::Normal)
//			));
//
//		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
//			vbuf.EmplaceBack(
//				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
//				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
//				);
//		}
//
//		//�C���f�b�N�X�o�b�t�@�̐ݒ�
//		std::vector<unsigned short> indices;
//		//�L���p�V�e�B�[�̐ݒ�
//		indices.reserve(mesh.mNumFaces * 3);
//		for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
//			const auto& face = mesh.mFaces[i];
//			assert(face.mNumIndices == 3);
//			indices.push_back(face.mIndices[0]);
//			indices.push_back(face.mIndices[1]);
//			indices.push_back(face.mIndices[2]);
//		}
//
//		std::vector<std::unique_ptr<Bindable>>bindablePtrs;
//
//		bindablePtrs.push_back(std::make_unique<VertexBuffer>(gfx, vbuf));
//		bindablePtrs.push_back(std::make_unique<IndexBuffer>(gfx, indices));
//
//		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cos");
//		auto pvsbc = pvs->GetByteCode();
//		bindablePtrs.push_back(std::move(pvs));
//
//		bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));
//
//		bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));
//
//		struct PSMaterialConstant {
//			DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
//			float specularIntensity = 0.6f;
//			float specularPower = 30.0f;
//			float padding[3];
//		}psConst;
//
//		bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, psConst, 1u));
//
//		return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
//	}
//
//	std::unique_ptr<Node>ParseNode(const aiNode& node) {
//		namespace dx = DirectX;
//		const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
//			reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
//			));
//
//		std::vector<Mesh*> curMeshPtrs;
//		curMeshPtrs.reserve(node.mNumMeshes);
//		for (size_t i = 0; i < node.mNumMeshes; i++) {
//			const auto meshIdx = node.mMeshes[i];
//			//at(meshIdx) = meshIdx�Ԗڂ̗v�f�ɃA�N�Z�X
//			curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
//		}
//
//		auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);
//		for (size_t i = 0; i < node.mNumChildren; i++) {
//			pNode->AddChild(ParseNode(*node.mChildren[i]));
//		}
//		return pNode;
//	}
//
//	void Draw(Graphics& gfx, DirectX::FXMMATRIX transform)const {
//		pRoot->Darw(gfx, transform);
//	}
//
//private:
//	std::unique_ptr<Node> pRoot;
//	std::vector < std::unique_ptr<Mesh>>meshPtrs;
//};

#endif