#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include"Graphics.h"
#include<DirectXMath.h>

class Bindable;

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
	virtual void Update(float dt)noexcept = 0;
	virtual ~Drawable() = default;
protected:
	//�e�I�u�W�F�N�g�̃o�C���h
	void AddBind(std::unique_ptr<Bindable> bind)noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf)noexcept;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds()const noexcept = 0;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};

#endif