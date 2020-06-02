#ifndef _DRAWABLEBASE_H_
#define _DRAWABLEBASE_H_

#include"Drawable.h"
#include"../Bindable/IndexBuffer.h"

//�e�I�u�W�F�N�g�����ʂ��Ď�����ۑ�����N���X
template<class T>
class DrawableBase :public Drawable
{
protected:
	//���g���󂩂ǂ������m�F����(�������m�F�B��łȂ��Ȃ�true)
	static bool IsStaticInitialized() noexcept {
		return !staticBinds.empty();
	}

	//Bind������̂�o�^����
	static void AddStaticBind(std::unique_ptr <Bind::Bindable> bind)noexcept {
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}

	//IndexBuffer��o�^����(pIndexBuffer�͊e�p���悪�����Ă��� static�ł͂Ȃ�)
	void AddStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf)noexcept {
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	void SetIndexFromStatic()noexcept {
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds) {
			if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get())) {
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}

private:
	//Bind����Ă���C���^�[�t�F�[�X��vector��Ԃ� = Drawable�̏������z�֐�
	const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds()const noexcept override {
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

//static�ϐ��̏������������ł��Ă���
//template��static�������ꍇ�A�w�b�_�̊O�ŏ����� 
//�e���v���[�g�ɂ����static��񂪕������
template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;

#endif