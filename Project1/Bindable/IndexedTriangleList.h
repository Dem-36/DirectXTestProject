#ifndef _INDEXEDTRIANGLELIST_H_
#define _INDEXEDTRIANGLELIST_H_

#include<vector>
#include<DirectXMath.h>

//�W�I���g�����쐬��A���_���ƃC���f�b�N�X����ۑ�����
template<class T>
class IndexedTriangleList {
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices)
		:vertices(std::move(verts_in)),
		indices(std::move(indices)) {
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	//�e���_�̃��f����ԏ�̈ʒu��傫���A��]�����߂�
	void Transform(DirectX::FXMMATRIX matrix) {
		//���_�̐�������
		for (auto& v : vertices) {
			//�ϊ� position = XMFLOAT3
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.position);
			//�������ŋ��߂�XMVECTOR��XMFLOAT3�ɕϊ�����
			//XMVector3Transform��XMVECTOR XMMATRIX����Z����
			DirectX::XMStoreFloat3(
				&v.position,
				DirectX::XMVector3Transform(pos, matrix)
				);
		}
	}

	//�擾�������_�ʒu����@�������߂�
	void SetNormalsIndependentFlat()noexcept {
		using namespace DirectX;
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t i = 0; i < indices.size(); i += 3) {
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.position);
			const auto p1 = XMLoadFloat3(&v1.position);
			const auto p2 = XMLoadFloat3(&v2.position);

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			//�A�h���X���擾���Ă��邽�߁Aindices�̒��g���ύX�����
			XMStoreFloat3(&v0.normal, n);
			XMStoreFloat3(&v1.normal, n);
			XMStoreFloat3(&v2.normal, n);
		}
	}

public:
	//�����ɕۑ�����钸�_���̓��f�����W
	std::vector<T> vertices;
	//�C���f�b�N�X�o�b�t�@
	std::vector<unsigned short> indices;
};

#endif