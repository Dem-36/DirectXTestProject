#include "TransformCbuf.h"

namespace Bind
{
	//staticの定数バッファ初期化
	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::vcbuf;

	//コンストラクタ
	//定数バッファはモデル行列
	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
		:parent(parent)
	{
		//情報を共有する
		if (!vcbuf) {
			vcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	//頂点定数バッファを更新後、
	//それを描画パイプラインにバインドする(TransformCbuf事態は別にバインドを行わない)
	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{
		const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
		const Transforms transform = {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
			),
		};
		//定数バッファ更新 モデルの位置や大きさ、回転情報を更新する
		//XMMatrixTransposeで転置行列にする
		vcbuf->Update(gfx, transform);
		//定数バッファをパイプラインにバインド
		vcbuf->Bind(gfx);
	}
}