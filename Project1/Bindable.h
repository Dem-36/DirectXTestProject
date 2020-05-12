#ifndef _BINDABLE_H_
#define _BINDABLE_H_

#include"Graphics.h"

//�C���^�[�t�F�[�X
class Bindable {
public:
	//�p�C�v���C���Ƀo�C���h������
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx)noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx)noexcept;
	static DxgiInfoManager& GetInfoManager(Graphics& gfx)noexcept;
};

#endif