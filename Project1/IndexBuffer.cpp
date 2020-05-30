#include "IndexBuffer.h"
#include"WinExceptionMacro.h"

namespace Bind
{
	//�R���X�g���N�^
	//�C���f�b�N�X�o�b�t�@�̍쐬(�`�掞�ɕK�v�Ȓ��_���)
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
		:count((UINT)indices.size())
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC ibd = {};

		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;               //�p�C�v���C���̃o�C���h����w��
		ibd.Usage = D3D11_USAGE_DEFAULT;                       //�o�b�t�@�ł̃A�N�Z�X���@�w��
		ibd.CPUAccessFlags = 0u;                               //���\�[�X�ɑ΂��Ă�CPU�A�N�Z�X�̕��@�w��
		ibd.MiscFlags = 0u;                                    //���̑��I�v�V����
		ibd.ByteWidth = UINT(count * sizeof(unsigned short));  //�o�b�t�@�̃T�C�Y�w��
		ibd.StructureByteStride = sizeof(unsigned short);      //�\���̂̃T�C�Y �o�b�t�@���\���̂Ƃ��Ċm�ۂ���ꍇ�Ɏg�p

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = indices.data();                     //�o�b�t�@������������f�[�^���w��
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &initData, &pIndexBuffer));
	}

	//�p�C�v���C���Ƀo�C���h����
	void IndexBuffer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	//�g�p���钸�_����Ԃ�
	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}