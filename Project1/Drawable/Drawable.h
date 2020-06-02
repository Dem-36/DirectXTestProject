#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include"../Graphics.h"
#include<DirectXMath.h>

namespace Bind {
	class Bindable;
	class IndexBuffer;
}

//�e�I�u�W�F�N�g�����ׂ�����ۑ�����N���X
class Drawable
{
	//DrawableBase�N���X��Drawable�N���X��private�֐����g����
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	//���f���s���Ԃ�
	virtual DirectX::XMMATRIX GetTransformXM()const noexcept = 0;
	//�`�揈��
	void Draw(Graphics& gfx)const noexcept;
	//�萔�o�b�t�@�X�V
	virtual void Update(float dt)noexcept {}
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable()noexcept {
		for (auto& pb : binds) {
			if (auto pt = dynamic_cast<T*>(pb.get())) {
				return pt;
			}
		}
		return nullptr;
	}
	//�e�I�u�W�F�N�g�̃o�C���h
	void AddBind(std::unique_ptr<Bind::Bindable> bind)noexcept;
	void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf)noexcept;
private:
	virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds()const noexcept = 0;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bind::Bindable>> binds;
};

#endif