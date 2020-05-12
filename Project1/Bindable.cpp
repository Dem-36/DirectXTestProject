#include "Bindable.h"

//Graphicsがfriendクラスのため、privateの変数を使用可能
//一方的なアクセス Bindable -> Graphics 逆はできない

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
	return gfx.infoManager;
}
