#include "IndexBuffer.h"
#include"WinExceptionMacro.h"

//�R���X�g���N�^
//�C���f�b�N�X�o�b�t�@�̍쐬(�`�掞�ɕK�v�Ȓ��_���)
IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
	:count((UINT)indices.size())
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd = {};

	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices.data();
	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &initData, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
