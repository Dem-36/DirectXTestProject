#include"Topology.h"

//�R���X�g���N�^(�v���~�e�B�u�w��)
Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:type(type)
{
}

//�v���~�e�B�u���o�C���h����
void Topology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
