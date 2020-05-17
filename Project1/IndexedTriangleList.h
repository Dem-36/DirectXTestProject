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
public:
	//�����ɕۑ�����钸�_���̓��f�����W
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};

#endif