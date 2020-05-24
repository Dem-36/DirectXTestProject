#ifndef _INDEXEDTRIANGLELIST_H_
#define _INDEXEDTRIANGLELIST_H_

#include<vector>
#include<DirectXMath.h>

//ジオメトリを作成後、頂点情報とインデックス情報を保存する
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

	//各頂点のモデル空間上の位置や大きさ、回転を求める
	void Transform(DirectX::FXMMATRIX matrix) {
		//頂点の数だけ回す
		for (auto& v : vertices) {
			//変換 position = XMFLOAT3
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.position);
			//第二引数で求めたXMVECTORをXMFLOAT3に変換する
			//XMVector3TransformでXMVECTOR XMMATRIXを乗算する
			DirectX::XMStoreFloat3(
				&v.position,
				DirectX::XMVector3Transform(pos, matrix)
				);
		}
	}

	//取得した頂点位置から法線を求める
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

			//アドレスを取得しているため、indicesの中身が変更される
			XMStoreFloat3(&v0.normal, n);
			XMStoreFloat3(&v1.normal, n);
			XMStoreFloat3(&v2.normal, n);
		}
	}

public:
	//ここに保存される頂点情報はモデル座標
	std::vector<T> vertices;
	//インデックスバッファ
	std::vector<unsigned short> indices;
};

#endif