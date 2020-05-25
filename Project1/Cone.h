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

	template<class T>
	static IndexedTriangleList<T> MakeTesselatedIndependentFaces(int longDiv) {
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<T> vertices;

		const auto iCone = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++) {
			const float thetas[] = {
				longitudeAngle * iLong,
				longitudeAngle * (((iLong + 1) == longDiv) ? 0 : (iLong + 1))
			};
			//末尾に構造体追加
			vertices.emplace_back();
			//末尾の要素の参照
			vertices.back().position = { 0.0f,0.0f,1.0f };
			for (auto theta : thetas) {
				vertices.emplace_back();
				const auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(theta));
				dx::XMStoreFloat3(&vertices.back().position, v);
			}
		}

		const auto iBaseCenter = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f };
		const auto iBaseEdge = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++) {
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * iLong));
			dx::XMStoreFloat3(&vertices.back().position, v);
		}

		std::vector<unsigned short> indices;

		for (unsigned short i = 0; i < longDiv * 3; i++) {
			indices.push_back(i + iCone);
		}

		for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
			indices.push_back(iBaseCenter);
			indices.push_back((iLong + 1) % longDiv + iBaseEdge);
			indices.push_back(iLong + iBaseEdge);
		}

		return {
			std::move(vertices),
			std::move(indices)
		};
	}
};

#endif