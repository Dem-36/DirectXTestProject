#include "TransformCbuf.h"

namespace Bind
{
	//static�̒萔�o�b�t�@������
	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::vcbuf;

	//�R���X�g���N�^
	//�萔�o�b�t�@�̓��f���s��
	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
		:parent(parent)
	{
		//�������L����
		if (!vcbuf) {
			vcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	//���_�萔�o�b�t�@���X�V��A
	//�����`��p�C�v���C���Ƀo�C���h����(TransformCbuf���Ԃ͕ʂɃo�C���h���s��Ȃ�)
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
		//�萔�o�b�t�@�X�V ���f���̈ʒu��傫���A��]�����X�V����
		//XMMatrixTranspose�œ]�u�s��ɂ���
		vcbuf->Update(gfx, transform);
		//�萔�o�b�t�@���p�C�v���C���Ƀo�C���h
		vcbuf->Bind(gfx);
	}
}