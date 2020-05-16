#ifndef _INDEXEDTRIANGLELIST_H_
#define _INDEXEDTRIANGLELIST_H_

#include<vector>
#include<DirectXMath.h>

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

	void Transform(DirectX::FXMMATRIX matrix) {
		for (auto& v : vertices) {
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.position);
			DirectX::XMStoreFloat3(
				&v.position,
				DirectX::XMVector3Transform(pos, matrix)
				);
		}
	}
public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};

#endif