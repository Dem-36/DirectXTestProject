#ifndef _CUBE_H_
#define _CUBE_H_

#include"IndexedTriangleList.h"
#include<DirectXMath.h>

class Cube {
public:
	//Cubeのジオメトリ作成
	template<class T>
	static IndexedTriangleList<T> Make() {
		namespace dx = DirectX;

		//コンパイル時に定数化
		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(8);
		vertices[0].position = { -side,-side,-side };
		vertices[1].position = { side,-side,-side };
		vertices[2].position = { -side,side,-side };
		vertices[3].position = { side,side,-side };
		vertices[4].position = { -side,-side,side };
		vertices[5].position = { side,-side,side };
		vertices[6].position = { -side,side,side };
		vertices[7].position = { side,side,side };

		return{
			std::move(vertices),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}

	template<class T>
	static IndexedTriangleList<T> MakeSkinned() {
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(14);

		vertices[0].position = { -side,-side,-side };
		vertices[0].uv = { 2.0f / 3.0f, 0.0f / 4.0f };
		vertices[1].position = { side,-side,-side };
		vertices[1].uv = { 1.0f / 3.0f, 0.0f / 4.0f };
		vertices[2].position = { -side,side,-side };
		vertices[2].uv = { 2.0f / 3.0f, 1.0f / 4.0f };
		vertices[3].position = { side,side,-side };
		vertices[3].uv = { 1.0f / 3.0f, 1.0f / 4.0f };
		vertices[4].position = { -side,-side,side };
		vertices[4].uv = { 2.0f / 3.0f, 3.0f / 4.0f };
		vertices[5].position = { side,-side,side };
		vertices[5].uv = { 1.0f / 3.0f, 3.0f / 4.0f };
		vertices[6].position = { -side,side,side };
		vertices[6].uv = { 2.0f / 3.0f, 2.0f / 4.0f };
		vertices[7].position = { side,side,side };
		vertices[7].uv = { 1.0f / 3.0f, 2.0f / 4.0f };
		vertices[8].position = { -side,-side,-side };
		vertices[8].uv = { 2.0f / 3.0f, 4.0f / 4.0f };
		vertices[9].position = { side,-side,-side };
		vertices[9].uv = { 1.0f / 3.0f, 4.0f / 4.0f };
		vertices[10].position = { -side,-side,-side };
		vertices[10].uv = { 3.0f / 3.0f, 1.0f / 4.0f };
		vertices[11].position = { -side,-side,side };
		vertices[11].uv = { 3.0f / 3.0f, 2.0f / 4.0f };
		vertices[12].position = { side,-side,-side };
		vertices[12].uv = { 0.0f / 3.0f, 1.0f / 4.0f };
		vertices[13].position = { side,-side,-side };
		vertices[13].uv = { 0.0f / 3.0f, 2.0f / 4.0f };

		return {
			std::move(vertices),
			{
				0,1,2, 2,3,1,
				4,8,5, 5,8,9,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				2,10,11, 2,11,6,
				12,3,7, 12,7,13
			}
		};
	}

	template<class T>
	static IndexedTriangleList<T> MakeIndependent() {
		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(24);
		vertices[0].position = { -side,-side,-side };// 0 near side
		vertices[1].position = { side,-side,-side };// 1
		vertices[2].position = { -side,side,-side };// 2
		vertices[3].position = { side,side,-side };// 3
		vertices[4].position = { -side,-side,side };// 4 far side
		vertices[5].position = { side,-side,side };// 5
		vertices[6].position = { -side,side,side };// 6
		vertices[7].position = { side,side,side };// 7
		vertices[8].position = { -side,-side,-side };// 8 left side
		vertices[9].position = { -side,side,-side };// 9
		vertices[10].position = { -side,-side,side };// 10
		vertices[11].position = { -side,side,side };// 11
		vertices[12].position = { side,-side,-side };// 12 right side
		vertices[13].position = { side,side,-side };// 13
		vertices[14].position = { side,-side,side };// 14
		vertices[15].position = { side,side,side };// 15
		vertices[16].position = { -side,-side,-side };// 16 bottom side
		vertices[17].position = { side,-side,-side };// 17
		vertices[18].position = { -side,-side,side };// 18
		vertices[19].position = { side,-side,side };// 19
		vertices[20].position = { -side,side,-side };// 20 top side
		vertices[21].position = { side,side,-side };// 21
		vertices[22].position = { -side,side,side };// 22
		vertices[23].position = { side,side,side };// 23

		return {
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}

	template<class T>
	static IndexedTriangleList<T> MakeIndependentTextured()
	{
		auto itl = MakeIndependent<T>();

		itl.vertices[0].uv = { 0.0f,0.0f };
		itl.vertices[1].uv = { 1.0f,0.0f };
		itl.vertices[2].uv = { 0.0f,1.0f };
		itl.vertices[3].uv = { 1.0f,1.0f };
		itl.vertices[4].uv = { 0.0f,0.0f };
		itl.vertices[5].uv = { 1.0f,0.0f };
		itl.vertices[6].uv = { 0.0f,1.0f };
		itl.vertices[7].uv = { 1.0f,1.0f };
		itl.vertices[8].uv = { 0.0f,0.0f };
		itl.vertices[9].uv = { 1.0f,0.0f };
		itl.vertices[10].uv = { 0.0f,1.0f };
		itl.vertices[11].uv = { 1.0f,1.0f };
		itl.vertices[12].uv = { 0.0f,0.0f };
		itl.vertices[13].uv = { 1.0f,0.0f };
		itl.vertices[14].uv = { 0.0f,1.0f };
		itl.vertices[15].uv = { 1.0f,1.0f };
		itl.vertices[16].uv = { 0.0f,0.0f };
		itl.vertices[17].uv = { 1.0f,0.0f };
		itl.vertices[18].uv = { 0.0f,1.0f };
		itl.vertices[19].uv = { 1.0f,1.0f };
		itl.vertices[20].uv = { 0.0f,0.0f };
		itl.vertices[21].uv = { 1.0f,0.0f };
		itl.vertices[22].uv = { 0.0f,1.0f };
		itl.vertices[23].uv = { 1.0f,1.0f };

		return itl;
	}
};

#endif