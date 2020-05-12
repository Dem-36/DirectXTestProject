#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:vcbuf(gfx),parent(parent)
{
}

//頂点定数バッファを更新後、
//それを描画パイプラインにバインドする(TransformCbuf事態は別にバインドを行わない)
void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	vcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
			));
	vcbuf.Bind(gfx);
}
