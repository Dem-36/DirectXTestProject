#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:vcbuf(gfx),parent(parent)
{
}

//���_�萔�o�b�t�@���X�V��A
//�����`��p�C�v���C���Ƀo�C���h����(TransformCbuf���Ԃ͕ʂɃo�C���h���s��Ȃ�)
void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	vcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
			));
	vcbuf.Bind(gfx);
}
