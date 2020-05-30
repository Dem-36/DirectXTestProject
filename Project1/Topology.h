#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_

#include"Bindable.h"

namespace Bind 
{
	//�v���~�e�B�u�̎w����s���N���X
	class Topology :public Bindable {
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx)noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}

#endif