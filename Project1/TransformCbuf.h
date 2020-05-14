#ifndef _TRANSFORMCBUF_H_
#define _TRANSFORMCBUF_H_

#include"ConstantBuffer.h"
#include"Drawable.h"
#include<DirectXMath.h>

class TransformCbuf:public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx)noexcept override;
private:
	//Box�Ԃŋ��ʂ�Transform������(�`�掞�ɒl��ύX���Ă���̂œ����ɉe���͂Ȃ�)
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Drawable& parent;
};

#endif