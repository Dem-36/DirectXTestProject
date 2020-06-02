#ifndef _TRANSFORMCBUF_H_
#define _TRANSFORMCBUF_H_

#include"ConstantBuffer.h"
#include"../Drawable/Drawable.h"
#include<DirectXMath.h>

namespace Bind
{
	//���f���s����w�肷��
	class TransformCbuf :public Bindable
	{
	private:
		struct Transforms {
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProjection;
		};
	public:
		TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
		//���_�萔�o�b�t�@���p�C�v���C���Ƀo�C���h����
		void Bind(Graphics& gfx)noexcept override;
	private:
		//���ʂ�Transform������(�`�掞�ɒl��ύX���Ă���̂œ����ɉe���͂Ȃ�)
		static std::unique_ptr<VertexConstantBuffer<Transforms>> vcbuf;
		const Drawable& parent;
	};
}
#endif