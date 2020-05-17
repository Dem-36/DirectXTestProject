#ifndef _TRANSFORMCBUF_H_
#define _TRANSFORMCBUF_H_

#include"ConstantBuffer.h"
#include"Drawable.h"
#include<DirectXMath.h>

//���f���s����w�肷��
class TransformCbuf:public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	//���_�萔�o�b�t�@���p�C�v���C���Ƀo�C���h����
	void Bind(Graphics& gfx)noexcept override;
private:
	//���ʂ�Transform������(�`�掞�ɒl��ύX���Ă���̂œ����ɉe���͂Ȃ�)
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Drawable& parent;
};

#endif