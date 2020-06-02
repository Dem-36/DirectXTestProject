#ifndef _TRANSFORMCBUF_H_
#define _TRANSFORMCBUF_H_

#include"ConstantBuffer.h"
#include"../Drawable/Drawable.h"
#include<DirectXMath.h>

namespace Bind
{
	//モデル行列を指定する
	class TransformCbuf :public Bindable
	{
	private:
		struct Transforms {
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProjection;
		};
	public:
		TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
		//頂点定数バッファをパイプラインにバインドする
		void Bind(Graphics& gfx)noexcept override;
	private:
		//共通のTransformを持つ(描画時に値を変更しているので動きに影響はない)
		static std::unique_ptr<VertexConstantBuffer<Transforms>> vcbuf;
		const Drawable& parent;
	};
}
#endif