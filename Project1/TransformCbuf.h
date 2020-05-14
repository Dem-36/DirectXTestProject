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
	//BoxŠÔ‚Å‹¤’Ê‚ÌTransform‚ğ‚Â(•`‰æ‚É’l‚ğ•ÏX‚µ‚Ä‚¢‚é‚Ì‚Å“®‚«‚É‰e‹¿‚Í‚È‚¢)
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Drawable& parent;
};

#endif