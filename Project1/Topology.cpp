#include"Topology.h"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:type(type)
{
}

//�v���~�e�B�u���o�C���h����
void Topology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
