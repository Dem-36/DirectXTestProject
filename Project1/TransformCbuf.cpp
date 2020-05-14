#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:parent(parent)
{
	//Box�ŏ������L����
	if (!vcbuf) {
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

//���_�萔�o�b�t�@���X�V��A
//�����`��p�C�v���C���Ƀo�C���h����(TransformCbuf���Ԃ͕ʂɃo�C���h���s��Ȃ�)
void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	vcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
			));
	vcbuf->Bind(gfx);
}

//static�̒萔�o�b�t�@�������E�E�E�H
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::vcbuf;
