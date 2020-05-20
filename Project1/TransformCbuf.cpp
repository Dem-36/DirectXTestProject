#include "TransformCbuf.h"

//static�̒萔�o�b�t�@������
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::vcbuf;

//�R���X�g���N�^
//�萔�o�b�t�@�̓��f���s��
TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:parent(parent)
{
	//�������L����
	if (!vcbuf) {
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

//���_�萔�o�b�t�@���X�V��A
//�����`��p�C�v���C���Ƀo�C���h����(TransformCbuf���Ԃ͕ʂɃo�C���h���s��Ȃ�)
void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	//�萔�o�b�t�@�X�V ���f���̈ʒu��傫���A��]�����X�V����
	//XMMatrixTranspose�œ]�u�s��ɂ���
	vcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() *
			gfx.GetCamera() *
			gfx.GetProjection()
			));
	//�萔�o�b�t�@���p�C�v���C���Ƀo�C���h
	vcbuf->Bind(gfx);
}