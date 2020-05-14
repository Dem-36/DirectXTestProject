#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:parent(parent)
{
	//Boxで情報を共有する
	if (!vcbuf) {
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

//頂点定数バッファを更新後、
//それを描画パイプラインにバインドする(TransformCbuf事態は別にバインドを行わない)
void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	vcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
			));
	vcbuf->Bind(gfx);
}

//staticの定数バッファ初期化・・・？
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::vcbuf;
