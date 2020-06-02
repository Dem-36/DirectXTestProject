#include "Bindable.h"
#include<stdexcept>

namespace Bind
{
	//Graphics��friend�N���X�̂��߁Aprivate�̕ϐ����g�p�\
	//����I�ȃA�N�Z�X Bindable -> Graphics �t�͂ł��Ȃ�

	ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
	{
		return gfx.pContext.Get();
	}

	ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
	{
		return gfx.pDevice.Get();
	}

	DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept
	{
#ifndef NDEBUG
		return gfx.infoManager;
#else
		throw std::logic_error("You Fucked up! (tried to access gfx.infoManager in Release config)");
#endif
	}
}