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
public:
	//ここに保存される頂点情報はモデル座標
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};

#endif