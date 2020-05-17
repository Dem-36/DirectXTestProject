#include"Topology.h"

//コンストラクタ(プリミティブ指定)
Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:type(type)
{
}

//プリミティブをバインドする
void Topology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
