#ifndef _CONE_H_
#define _CONE_H_

#include"IndexedTriangleList.h"
#include<DirectXMath.h>
#include"WinMath.h"

//Coneのジオメトリを作成
class Cone {
public:
	//T = 頂点構造体
	//Coneのジオメトリを作成するlongDivは三角錐の底辺頂点数
	template<class T>
	static IndexedTriangleList<T> MakeTesselated(int longDiv) {

		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		//base vertices
		std::vector<T> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++) {
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
			dx::XMStoreFloat3(&vertices.back().position, v);
		}

		//center
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		//the tip :darkness
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);

		//base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		//cone indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iTip);
		}

		return IndexedTriangleList<T>(std::move(vertices), std::move(indices));
	}

	//Coneのジオメトリを作成
	template<class T>
	static IndexedTriangleList<T> Make() {
		return MakeTesselated<T>(24);
	}
};

#endif